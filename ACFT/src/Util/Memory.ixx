export module Memory;

export import <memory>;

export template<typename F, typename S = F>
struct BiComplex
{
	F first;
	S second;
};

export template<typename F, typename S = F, typename T = S>
struct TriComplex
{
	F first;
	S second;
	T third;
};

template<size_t Size, size_t Align>
struct alignas(Align) _MemoryTraitBlock
{
	char padding[Size];
};

export template<typename T>
struct MemoryTrait
{
	using type = _MemoryTraitBlock<sizeof(T), alignof(T)>;
};

export template<typename T>
using MemoryTraitType = MemoryTrait<T>::type;

// An allocator manages a block of memory, including allocating it and releasing it.
export template<typename T>
concept Allocator = requires(T t, size_t n, std::decay_t<std::remove_reference_t<T>>::value_type * ptr)
{
	{ t.allocate(n) } -> std::convertible_to<typename std::decay_t<std::remove_reference_t<T>>::value_type*>;
	t.deallocate(ptr, n);
};

export template<Allocator Alloc>
struct AllocatorTraits
{
	using value_type = Alloc::value_type;

	static value_type* allocate(Alloc& allocator, size_t n)
	{
		return allocator.allocate(n);
	}

	static void deallocate(Alloc& allocator, value_type* ptr, size_t n)
	{
		allocator.deallocate(ptr, n);
	}
};

// A deleter ends an object's lifecycle, but don' necessarily release the memory.
// In other words, it's not operator delete's alias.
export template<typename T>
concept Deleter = requires(T t, std::decay_t<std::remove_reference_t<T>>::value_type * ptr)
{
	t(ptr);
};

export template<typename T>
struct DefaultAllocator
{
	using value_type = T;

	constexpr DefaultAllocator() = default;

	T* allocate(size_t n) const
	{
		return std::launder(
			reinterpret_cast<T*>(
				::operator new(sizeof(T), std::align_val_t{ alignof(T) })
				)
		);
	}

	void deallocate(T* ptr, size_t n) const
	{
		::operator delete(std::launder(ptr), std::align_val_t{ alignof(T) });
	}
};

export template<typename T>
constinit DefaultAllocator<T> default_allocator_v{};

export template<typename T>
struct DefaultDeleter
{
	using value_type = T;

	constexpr DefaultDeleter() = default;

	void operator()(value_type* ptr) const noexcept
	{
		if constexpr (!std::is_trivially_destructible_v<value_type>)
		{
			ptr->~T();
		}
	}
};

export template<typename T>
constinit DefaultDeleter<T> default_deleter_v{};

export template<typename T>
class Scope
{
public:
	using value_type = T;

	template<typename U>
	friend class Ref;

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

	template<typename U = std::enable_if_t<!std::is_void_v<T>>>
	auto& operator*() const noexcept
	{
		return *m_Obj;
	}

	T* operator->() const noexcept
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

	size_t operator()(const Scope<T>& scope) const noexcept
	{
		return std::hash<typename Scope<T>::value_type*>{}(scope.get());
	}
};

export template<typename T, typename... Args>
Scope<T> MakeScope(Args&&... args)
{
	T* raw_ptr = new T(std::forward<Args>(args)...);
	return Scope<T>{ raw_ptr };
}

struct _Ctrl_Base
{
	std::atomic<long> refs;
	std::atomic<long> weak_refs;

	_Ctrl_Base() : refs(1), weak_refs(1) {}
	virtual ~_Ctrl_Base() = default;

	virtual void DestroyObject() = 0;
	virtual void DeleteThis() = 0;

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
			DeleteThis();
		}
	}
};

template<typename T>
struct _Ctrl_Small : public _Ctrl_Base
{
	T* obj;

	_Ctrl_Small(T* ptr) : obj(ptr) {}

	virtual void DestroyObject() override
	{
		delete obj;
	}

	virtual void DeleteThis() override
	{
		delete this;
	}
};

template<typename T, Allocator Alloc, Deleter Del = DefaultDeleter<T>>
struct _Ctrl : public _Ctrl_Base
{
	T* obj;
	Alloc* alloc;
	Del* del;

	_Ctrl(T* ptr, Alloc* allocator = nullptr, Del* deleter = nullptr)
		: obj(ptr), alloc(allocator), del(deleter)
	{
		if constexpr (std::is_same_v<Del, DefaultDeleter<T>>)
			del = &default_deleter_v<T>;
	}

	virtual void DestroyObject() override
	{
		if (del == nullptr)
		{
			delete obj;
		}
		else
		{
			(*del)(obj);
		}
	}

	virtual void DeleteThis() override
	{
		if (alloc == nullptr)
		{
			delete this;
		}
		else
		{
			alloc->deallocate(reinterpret_cast<Alloc::value_type*>(this), 1);
		}
	}
};

template<typename T, Allocator Alloc, Deleter Del = DefaultDeleter<T>>
	requires std::is_move_constructible_v<Alloc> || std::is_copy_constructible_v<Alloc>
struct _Ctrl_Owning_Allocator : public _Ctrl_Base
{
	T* obj;
	Alloc* alloc;
	Del* del;

	_Ctrl_Owning_Allocator(T* ptr) : obj(ptr), alloc(nullptr), del(nullptr) {}
	_Ctrl_Owning_Allocator(T* ptr, const Alloc& allocator, Del* deleter = nullptr) : obj(ptr), alloc(new Alloc(allocator)), del(deleter) {}
	_Ctrl_Owning_Allocator(T* ptr, Alloc&& allocator, Del* deleter = nullptr) : obj(ptr), alloc(new Alloc(std::move(allocator))), del(deleter) {}

	virtual ~_Ctrl_Owning_Allocator() override
	{
		if (alloc != nullptr)
			delete alloc;
	}

	virtual void DestroyObject() override
	{
		if (del == nullptr)
		{
			delete obj;
		}
		else
		{
			(*del)(obj);
		}
	}

	virtual void DeleteThis() override
	{
		if (alloc != nullptr)
		{
			alloc->deallocate(reinterpret_cast<Alloc::value_type*>(this), 1);
			delete alloc;
		}
		else
		{
			delete this;
		}
	}
};

template<typename T>
class View;

struct _CtrlMemoryTrait : public _Ctrl<int, DefaultAllocator<int>, DefaultDeleter<int>> {};

template<typename T>
using _RefInplaceBlockTypeTrait = BiComplex<_CtrlMemoryTrait, T>;

// Only makes sense when using AllocateMakeRef, which receives customized allocator.
export template<typename T>
struct RefInplaceBlockTraits
{
	using type = _RefInplaceBlockTypeTrait<T>;
	static constexpr size_t size = sizeof(type);
	static constexpr std::ptrdiff_t obj_offset = offsetof(type, type::second);
};

template<typename T>
class Ref;

export template<typename U, Allocator Alloc, typename... Args>
	requires std::is_same_v<typename std::decay_t<Alloc>::value_type, MemoryTraitType<typename RefInplaceBlockTraits<U>::type>>
inline Ref<U> AllocateMakeRef(Alloc&& allocator, Args&&... args) noexcept
{
	using block_t = MemoryTraitType<typename RefInplaceBlockTraits<U>::type>;
	using Ctrl_Type = _Ctrl<U, std::decay_t<Alloc>, DefaultDeleter<U>>;
	using Ctrl_A_Type = _Ctrl_Owning_Allocator<U, std::decay_t<Alloc>, DefaultDeleter<U>>;

	char* block = reinterpret_cast<char*>(allocator.allocate(1));
	U* obj_ptr = reinterpret_cast<U*>(block + RefInplaceBlockTraits<U>::obj_offset);

	_Ctrl_Base* block_ptr;
	if constexpr (std::is_lvalue_reference_v<Alloc>)
	{
		block_ptr = std::construct_at(reinterpret_cast<Ctrl_Type*>(block), obj_ptr, &allocator);
	}
	else
	{
		block_ptr = std::construct_at(reinterpret_cast<Ctrl_A_Type*>(block), obj_ptr, std::forward<Alloc>(allocator));
	}

	std::construct_at(obj_ptr, std::forward<Args>(args)...);
	Ref<U> ret{};
	ret.m_Ctrl = block_ptr;
	ret.m_Obj = obj_ptr;

	return ret;
}

export template<typename U, Allocator Alloc, Deleter Del, typename... Args>
	requires std::is_same_v<typename std::decay_t<Alloc>::value_type, MemoryTraitType<typename RefInplaceBlockTraits<U>::type>>
inline Ref<U> AllocateMakeRef(Alloc&& allocator, Del& deleter, Args&&... args) noexcept
{
	using block_t = MemoryTraitType<typename RefInplaceBlockTraits<U>::type>;
	using Ctrl_Type = _Ctrl<U, std::decay_t<Alloc>, std::decay_t<Del>>;
	using Ctrl_A_Type = _Ctrl_Owning_Allocator<U, std::decay_t<Alloc>, std::decay_t<Del>>;

	char* block = reinterpret_cast<char*>(allocator.allocate(1));
	U* obj_ptr = reinterpret_cast<U*>(block + RefInplaceBlockTraits<U>::obj_offset);

	_Ctrl_Base* block_ptr;
	if constexpr (std::is_lvalue_reference_v<Alloc>)
	{
		block_ptr = std::construct_at(reinterpret_cast<Ctrl_Type*>(block), obj_ptr, &allocator, &deleter);
	}
	else
	{
		block_ptr = std::construct_at(reinterpret_cast<Ctrl_A_Type*>(block), obj_ptr, std::forward<Alloc>(allocator), &deleter);
	}

	std::construct_at(obj_ptr, std::forward<Args>(args)...);
	Ref<U> ret{};
	ret.m_Ctrl = block_ptr;
	ret.m_Obj = obj_ptr;

	return ret;
}

export template<typename U, typename... Args>
inline Ref<U> MakeRef(Args&&... args)
{
	using block_t = MemoryTraitType<typename RefInplaceBlockTraits<U>::type>;
	return AllocateMakeRef<U>(default_allocator_v<block_t>, std::forward<Args>(args)...);
}

export template<typename U, Deleter Del, typename... Args>
inline Ref<U> MakeRef(Del& deleter, Args&&... args)
{
	using block_t = MemoryTraitType<typename RefInplaceBlockTraits<U>::type>;
	return AllocateMakeRef<U>(default_allocator_v<block_t>, deleter, std::forward<Args>(args)...);
}

export template<typename T>
class Ref
{
public:
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

	template<typename U>
	friend class View;

	template<typename U>
	friend class Ref;

	/**
	 * Use a allocator to manage memory allocation of the whole Ref.
	 *
	 * \param allocator An allocator that supports allocating a memory block that is suitable for the control block and the object.
	 * \param ...args Arguments used to contruct the object.
	 * \return The Ref made.
	 */
	template<typename U, Allocator Alloc, typename... Args>
		requires std::is_same_v<typename std::decay_t<Alloc>::value_type, MemoryTraitType<typename RefInplaceBlockTraits<U>::type>>
	friend Ref<U> AllocateMakeRef(Alloc&& allocator, Args&&... args) noexcept;

	template<typename U, Allocator Alloc, Deleter Del, typename... Args>
		requires std::is_same_v<typename std::decay_t<Alloc>::value_type, MemoryTraitType<typename RefInplaceBlockTraits<U>::type>>
	friend Ref<U> AllocateMakeRef(Alloc&& allocator, Del& deleter, Args&&... args) noexcept;

	template<typename U, typename... Args>
	friend Ref<U> MakeRef(Args&&... args);

	template<typename U, Deleter Del, typename... Args>
	friend Ref<U> MakeRef(Del& deleter, Args&&... args);

public:
	Ref() noexcept : Ref(nullptr) {}
	Ref(std::nullptr_t) noexcept : m_Obj(nullptr), m_Ctrl(nullptr) {}
	explicit Ref(T* ptr) noexcept
		: m_Obj(ptr), m_Ctrl(new _Ctrl_Small(ptr))
	{
	}

	template<Deleter Del>
	explicit Ref(T* ptr, Del& deleter)
		: m_Obj(ptr), m_Ctrl(new _Ctrl(ptr, nullptr, &deleter))
	{
	}

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

	Ref(const View<T>& view)
		: m_Obj(view.m_Obj), m_Ctrl(view.m_Ctrl)
	{
		if (m_Ctrl != nullptr)
			m_Ctrl->Incref();
	}

	Ref(Scope<T>&& scope)
		: Ref(scope.release())
	{ }

	Ref<T>& operator=(const Ref<T>& other) noexcept
	{
		m_Obj = other.m_Obj;
		m_Ctrl = other.m_Ctrl;
		m_Ctrl->Incref();

		return *this;
	}

	Ref<T>& operator=(Ref<T>&& other) noexcept
	{
		m_Obj = other.m_Obj;
		m_Ctrl = other.m_Ctrl;
		other.m_Obj = nullptr;
		other.m_Ctrl = nullptr;

		return *this;
	}

	T* get() const noexcept
	{
		return m_Obj;
	}

	template<typename U = std::enable_if_t<!std::is_void_v<T>>>
	auto& operator*() const noexcept
	{
		return *m_Obj;
	}

	T* operator->() const noexcept
	{
		return m_Obj;
	}

	void reset() noexcept
	{
		Ref<T> temp = Ref<T>();
		swap(temp);
	}

	void reset(T* ptr) noexcept
	{
		m_Ctrl->Decref();
		m_Obj = ptr;
		if (ptr != nullptr)
			m_Ctrl = new _Ctrl_Small<T>();
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

private:
	T* m_Obj;
	_Ctrl_Base* m_Ctrl;
};

export template<typename T>
struct std::hash<Ref<T>>
{
	using is_transparent = int;

	size_t operator()(const Ref<T>& ref) const noexcept
	{
		return std::hash<typename Ref<T>::value_type*>{}(ref.get());
	}
};

export template<>
struct std::hash<Ref<void>>
{
	size_t operator()(const Ref<void>& ref) const noexcept
	{
		return std::hash<std::nullptr_t>{}(nullptr);
	}
};

export template<typename T>
class View
{
public:
	using value_type = T;

	template<typename U>
	friend class Ref;

	friend bool operator==(const View<T>& lhs, const View<T>& rhs) noexcept
	{
		return lhs.m_Obj == rhs.m_Obj;
	}

	friend bool operator<=>(const View<T>& lhs, const View<T>& rhs) noexcept
	{
		return lhs.m_Obj <=> rhs.m_Obj;
	}

	template<typename U>
	View(const Ref<U>& ref)
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

		return *this;
	}

	View<T>& operator=(const View<T>& other)
	{
		m_Ctrl = other.m_Ctrl;
		m_Obj = other.m_Obj;
		m_Ctrl->Incweak();

		return *this;
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
		ref_t ref{};
		return expired() ? ref_t{} : ref_t{ *this };
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
	size_t operator()(const View<T>& view) const noexcept
	{
		using value_t = View<T>::value_type;
		return std::hash<value_t*>{}(view.lock().get());
	}
};

export template<>
struct std::hash<View<void>>
{
	size_t operator()(const View<void>& view) const noexcept
	{
		return std::hash<std::nullptr_t>{}(nullptr);
	}
};

export template<typename T, Allocator Alloc>
View<T> MakeView(Ref<T>&& ref)
{
	return View<T>(ref);
}