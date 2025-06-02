export module Memory;

export import <memory>;

export template<typename T>
concept Allocator = requires(T t, size_t n)
{
	typename T::value_type;
	{ t.allocate(n) } -> std::convertible_to<typename T::value_type*>;
	t.deallocate(std::declval<typename T::value_type*>(), n);
};

export template<typename T>
class Scope
{
public:
	using value_type = T;

	friend bool operator==(const Scope<T>& lhs, const Scope<T>& rhs) noexcept
	{
		return lhs.m_Obj == rhs.m_Obj;
	}

	friend std::strong_ordering operator<=>(const Scope<T>& lhs, const Scope<T>& rhs) noexcept
	{
		return lhs.m_Obj <=> rhs.m_Obj;
	}

	friend bool operator==(const Scope<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.m_Obj == nullptr;
	}

	friend std::strong_ordering operator<=>(const Scope<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.m_Obj <=> nullptr;
	}

	Scope() noexcept : m_Obj(nullptr) {}
	explicit Scope(T* ptr) noexcept : m_Obj(ptr) {}
	Scope(const Scope<T>&) = delete;
	Scope(Scope<T>&& other) noexcept
		: m_Obj(other.m_Obj)
	{
		other.m_Obj = nullptr;
	}

	template<typename U>
		requires std::convertible_to<U*, T*>
	Scope(Scope<U>&& other) noexcept
		: m_Obj(static_cast<T*>(other.m_Obj))
	{
		other.m_Obj = nullptr;
	}

	Scope<T>& operator=(const Scope<T>&) = delete;
	Scope<T>& operator=(std::nullptr_t) noexcept
	{
		m_Obj = nullptr;
		return *this;
	}

	Scope<T>& operator=(Scope<T>&& other) noexcept
	{
		m_Obj = other.m_Obj;
		other.m_Obj = nullptr;
		return *this;
	}

	T* release() noexcept
	{
		T* ret = m_Obj;
		m_Obj = nullptr;
		return ret;
	}

	void reset(T* ptr) noexcept
	{
		FreeObj();
		m_Obj = ptr;
	}

	void swap(Scope<T>& other) noexcept
	{
		std::swap(m_Obj, other.m_Obj);
	}

	T* get() const noexcept
	{
		return m_Obj;
	}

	operator bool() const noexcept
	{
		return m_Obj != nullptr;
	}

	T& operator*() noexcept
	{
		return *m_Obj;
	}

	T* operator->() noexcept
	{
		return m_Obj;
	}

	~Scope()
	{
		FreeObj();
	}

private:
	void FreeObj()
	{
		if (m_Obj != nullptr)
		{
			delete m_Obj;
			m_Obj = nullptr;
		}
	}

private:
	T* m_Obj;
};

export template<typename T>
struct std::hash<Scope<T>>
{
	using is_transparent = int;

	size_t operator()(const Scope<T>& scope)
	{
		return std::hash<typename Scope<T>::value_type*>{}(scope.get());
	}
};

export template<typename T, typename... Args>
Scope<T>&& MakeScope(Args&&... args)
{
	T* raw_ptr = new T(std::forward<Args>(args)...);
	return Scope(raw_ptr);
}

struct _Ctrl_Base
{
	std::atomic<long> refs;
	std::atomic<long> weak_refs;

	_Ctrl_Base() : refs(1), weak_refs(1) {}
	virtual ~_Ctrl_Base() = default;

	virtual void DestroyObject() = 0;

	void Incref()
	{
		refs.fetch_add(1, std::memory_order_relaxed);
	}

	void Decref()
	{
		if (refs.fetch_sub(1, std::memory_order_acq_rel) == 1)
		{
			DestroyObject();
			Decweak();
		}
	}

	void Incweak()
	{
		weak_refs.fetch_add(1, std::memory_order_release);
	}

	void Decweak()
	{
		if (weak_refs.fetch_sub(1, std::memory_order_acq_rel) == 1)
		{
			delete this;
		}
	}

};

template<typename T, Allocator Alloc = std::allocator<T>>
struct _Ctrl : public _Ctrl_Base
{
	T* obj;
	Alloc* alloc;

	_Ctrl(T* ptr) : obj(ptr), alloc(nullptr) {}
	_Ctrl(T* ptr, Alloc* allocator) : obj(ptr), alloc(allocator) {}

	virtual ~_Ctrl() override {}

	virtual void DestroyObject() override
	{
		if (alloc == nullptr)
		{
			delete obj;
			return;
		}
		
		std::allocator_traits<Alloc>::destroy(*alloc, obj);
		std::allocator_traits<Alloc>::deallocate(*alloc, obj, 1);
	}
};

template<typename T, Allocator Alloc = std::allocator<T>>
	requires std::is_move_constructible_v<Alloc> || std::is_copy_constructible_v<Alloc>
struct _Ctrl_Owning_Allocator : public _Ctrl_Base
{
	T* obj;
	Alloc* alloc;

	_Ctrl_Owning_Allocator(T* ptr) : obj(ptr), alloc(nullptr) {}
	_Ctrl_Owning_Allocator(T* ptr, const Alloc& allocator) : obj(ptr), alloc(new Alloc(allocator)) {}
	_Ctrl_Owning_Allocator(T* ptr, Alloc&& allocator) : obj(ptr), alloc(new Alloc(std::move(allocator))) {}

	virtual ~_Ctrl_Owning_Allocator() override 
	{
		if (alloc != nullptr)
			delete alloc;
	}

	virtual void DestroyObject() override
	{
		if (alloc == nullptr)
		{
			delete obj;
			return;
		}

		std::allocator_traits<Alloc>::destroy(*alloc, obj);
		std::allocator_traits<Alloc>::deallocate(*alloc, obj, 1);
	}
};

template<typename T, typename C>
struct _Ctrl_Obj_Complex
{
	C ctrl;
	T obj;
};

template<typename T>
struct _Ref_Default_Implace_Allocator
{
	using value_type = T;

	T* allocate(size_t n) noexcept
	{
		return new T();
	}

	void deallocate(T* ptr, size_t n) noexcept
	{
		delete ptr;
	}
};

template<typename T>
class View;

export template<typename T>
class Ref
{
	using _Implace_Block_Type = _Ctrl_Obj_Complex<T, _Ctrl<T, std::allocator<int>>>;
public:
	static constexpr size_t implace_block_size = sizeof(_Implace_Block_Type);
	static constexpr std::ptrdiff_t implace_block_obj_offset = offsetof(_Implace_Block_Type, _Implace_Block_Type::obj);

	using value_type = T;

	using view_type = View<value_type>;

	friend bool operator==(const Ref<T>& lhs, const Ref<T>& rhs) noexcept
	{
		return lhs.m_Obj == rhs.m_Obj;
	}

	friend std::strong_ordering operator<=>(const Ref<T>& lhs, const Ref<T>& rhs) noexcept
	{
		return lhs.m_Obj <=> rhs.m_Obj;
	}

	friend bool operator==(const Ref<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.m_Obj == nullptr;
	}

	friend std::strong_ordering operator<=>(const Ref<T>& lhs, std::nullptr_t) noexcept
	{
		return lhs.m_Obj <=> nullptr;
	}

private:
	friend class view_type;

public:
	Ref(std::nullptr_t) noexcept : m_Obj(nullptr), m_Ctrl(nullptr) {}
	explicit Ref(T* ptr) noexcept
		: m_Obj(ptr), m_Ctrl(new _Ctrl(ptr))
	{ }

	Ref(const Ref<T>& other) noexcept
		: m_Obj(other.m_Obj), m_Ctrl(other.m_Ctrl)
	{
		if (m_Ctrl != nullptr)
			m_Ctrl->Incref();
	}

	template<typename U>
		requires std::convertible_to<U*, T*>
	Ref(const Ref<U>& other) noexcept
		: m_Obj(static_cast<T*>(other.m_Obj)), m_Ctrl(other.m_Ctrl)
	{
		if (m_Ctrl != nullptr)
			m_Ctrl->Incref();
	}

	Ref(Ref<T>&& other) noexcept
		: m_Obj(other.m_Obj), m_Ctrl(other.m_Ctrl)
	{
		other.m_Obj = nullptr;
		other.m_Ctrl = nullptr;
	}

	template<typename U>
		requires std::convertible_to<U*, T*>
	Ref(Ref<U>&& other) noexcept
		: m_Obj(static_cast<T*>(other.m_Obj)), m_Ctrl(other.m_Ctrl)
	{
		other.m_Obj = nullptr;
		other.m_Ctrl = nullptr;
	}

	Ref<T>& operator=(const Ref<T>& other) noexcept
	{
		m_Obj = other.m_Obj;
		m_Ctrl = other.m_Ctrl;
		m_Ctrl->Incref();
	}

	Ref<T>& operator=(Ref<T>&& other) noexcept
	{
		m_Obj = other.m_Obj;
		m_Ctrl = other.m_Ctrl;
		other.m_Obj = nullptr;
		other.m_Ctrl = nullptr;
	}

	T* get() const noexcept
	{
		return m_Obj;
	}

	T& operator*() noexcept
	{
		return *m_Obj;
	}

	T* operator->() noexcept
	{
		return m_Obj;
	}

	void reset() noexcept
	{
		swap(Ref<T>());
	}

	void reset(T* ptr) noexcept
	{
		m_Ctrl->Decref();
		m_Obj = ptr;
		if (ptr != nullptr)
			m_Ctrl = new _Ctrl<T>();
	}

	void swap(Ref<T>& other) noexcept
	{
		std::swap(m_Obj, other.m_Obj);
		std::swap(m_Ctrl, other.m_Ctrl);
	}

	operator bool() const noexcept
	{
		return m_Obj != nullptr;
	}

	~Ref()
	{
		if (m_Ctrl != nullptr)
			m_Ctrl->Decref();

		m_Obj = nullptr;
		m_Ctrl = nullptr;
	}

	template<typename T, typename... Args>
	friend Ref<T> MakeRef(Args&&... args)
	{
		T* raw_ptr = new T(std::forward<Args>(args)...);
		return Ref<T>(raw_ptr);
	}

	/**
	 * Use a allocator to manage memory allocation of the whole Ref.
	 * 
	 * \param allocator An allocator that supports allocating a memory block that is suitable for the control block and the object. 
	 * \param ...args Arguments used to contruct the object.
	 * \return The Ref made.
	 */
	template<typename T, Allocator Alloc, typename... Args>
	friend Ref<T> AllocateMakeRef(Alloc&& allocator, Args&&... args) noexcept
	{
		using allocator_t = std::decay_t<std::remove_reference_t<Alloc>>;
		using allocator_ref_t = std::remove_cv_t<Alloc>;

		Ref<T>::_Implace_Block_Type* block = std::allocator_traits<allocator_t>::allocate(allocator, 1);
		T* obj_ptr = static_cast<T*>(block + Ref<T>::implace_block_obj_offset);
		
		if constexpr (std::is_same_v<allocator_ref_t, allocator_t&>)
		{
			std::construct_at(static_cast<_Ctrl*>(block), obj_ptr, &allocator);
		}
		else
		{
			std::construct_at(static_cast<_Ctrl_Owning_Allocator*>(block), obj_ptr, std::forward<Alloc>(allocator));
		}

		std::construct_at(obj_ptr, std::forward<Args>(args)...);
		Ref<T> ret{};
		ret.m_Ctrl = static_cast<_Ctrl_Base*>(block);
		ret.m_Obj = obj_ptr;

		return ret;
	}

private:
	T* m_Obj;
	_Ctrl_Base* m_Ctrl;

private:
	static inline _Ref_Default_Implace_Allocator<_Ctrl_Obj_Complex<T, _Ctrl<T, std::allocator<int>>>> s_ImplaceAllocator;
};

export template<typename T>
struct std::hash<Ref<T>>
{
	using is_transparent = int;

	size_t operator()(const Ref<T>& ref) noexcept
	{
		return std::hash<typename Ref<T>::value_type*>{}(ref.get());
	}
};

export template<typename T>
class View
{
public:
	using value_type = T;

	friend bool operator==(const View<T>& lhs, const View<T>& rhs) noexcept
	{
		return lhs.m_Obj == rhs.m_Obj;
	}

	friend bool operator<=>(const View<T>&lhs, const View<T>&rhs) noexcept
	{
		return lhs.m_Obj <=> rhs.m_Obj;
	}

	View(const Ref<value_type>& ref)
		: m_Ctrl(ref.m_Ctrl), m_Obj(static_cast<void*>(ref.m_Obj))
	{
		m_Ctrl->Incweak();
	}

	View(const View<T>& other)
		: m_Ctrl(other.m_Ctrl), m_Obj(static_cast<void*>(other.m_Obj))
	{
		m_Ctrl->Incweak();
	}

	View(View<T>&& other)
		: m_Ctrl(other.m_Ctrl), m_Obj(static_cast<void*>(other.m_Obj))
	{
		other.m_Ctrl = nullptr;
		other.m_Obj = nullptr;
	}

	View<T>& operator=(const Ref<value_type>& ref)
	{
		m_Ctrl = ref.m_Ctrl;
		m_Obj = ref.m_Obj;
		m_Ctrl->Incweak();
	}

	View<T>& operator=(const View<T>& other)
	{
		m_Ctrl = other.m_Ctrl;
		m_Obj = other.m_Obj;
		m_Ctrl->Incweak();
	}

	void reset() noexcept
	{
		m_Ctrl->Decweak();
		m_Obj = nullptr;
		m_Ctrl = nullptr;
	}

	void swap(View<T>& other) noexcept
	{
		std::swap(m_Obj, other.m_Obj);
		std::swap(m_Ctrl, other.m_Ctrl);
	}

	long use_count() const noexcept
	{
		if (m_Obj == nullptr || m_Ctrl == nullptr)
			return 0;
		else
			return m_Ctrl->refs.load(std::memory_order_acquire);
	}

	bool expired() const noexcept
	{
		return use_count() == 0;
	}

	Ref<value_type> lock() const noexcept
	{
		using ref_t = Ref<value_type>;
		return expired() ? ref_t{} : ref_t{ static_cast<value_type*>(m_Obj), *m_Ctrl->allocator };
	}

	~View()
	{
		if (m_Ctrl != nullptr)
			m_Ctrl->Decweak();

		m_Ctrl = nullptr;
		m_Obj = nullptr;
	}


private:
	_Ctrl_Base* m_Ctrl;
	void* m_Obj;
};

export template<typename T>
struct std::hash<View<T>>
{
	size_t operator()(const View<T>& view) const
	{
		using value_t = View<T>::value_type;
		std::hash<value_t*>{}(view.lock().get());
	}
};

export template<typename T, Allocator Alloc>
View<T> MakeView(Ref<T>&& ref)
{
	return View<T>(ref);
}
