module;

#include <memory>
#include <variant>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <functional>

export module Types;

export import Memory;

export template<typename T>
concept Is_Ref_Or_Scope = requires(T t)
{
	typename T::element_type;
	{ t.get() } -> std::convertible_to<typename T::element_type*>;
};

export template<typename T>
concept Is_View = requires(T t)
{
	typename decltype(t.lock())::element_type;
	{ t.lock().get() } -> std::convertible_to<typename decltype(t.lock())::element_type*>;
};

export template<typename T>
concept PtrWrapper = Is_Ref_Or_Scope<T> || Is_View<T>;

export template<PtrWrapper Wrapper>
struct PtrWrapperTraits
{
private:
	static constexpr auto choose_type()
	{
		if constexpr (Is_View<Wrapper>)
		{
			return decltype(std::declval<Wrapper>().lock().get()){};
		}
		else
		{
			return typename Wrapper::element_type{};
		}
	}

public:
	using ele_type = decltype(choose_type());
};

export template<typename T>
struct ViewEqual
{
	bool operator()(const View<T>& one, const View<T>& other) const
	{
		auto va = one.lock();
		auto vb = other.lock();
		if (va && vb)
		{
			return va.get() == vb.get();
		}

		return false;
	}
};

export template<typename T1, typename T2>
Ref<T1> RefCast(Ref<T2>&& ref)
{
	return Ref<T1>(std::forward<Ref<T2>>(ref));
}

export template<typename T1, typename T2>
Scope<T1> ScopeCast(Scope<T2>&& scope)
{
	return Scope<T1>(std::forward<Scope<T2>>(scope));
}

export template<PtrWrapper Wrapper>
PtrWrapperTraits<Wrapper>::ele_type* GetRawPtr(const Wrapper& wrapper)
{
	if constexpr (Is_View<Wrapper>)
	{
		auto sptr = wrapper.lock();
		if (sptr)
		{
			return sptr.get();
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return wrapper.get();
	}
}

export using RenderObjectIdentifier = std::variant<std::monostate, unsigned int>;

export using ModelMatrix = glm::mat4;

export struct StringHashFunc
{
	using is_transparent = int;

	size_t operator()(const std::string& s) const
	{
		return std::hash<std::string>{}(s);
	}

	size_t operator()(const std::string_view& v) const
	{
		return std::hash<std::string_view>{}(v);
	}
};

export struct EqualFunc
{
	using is_transparent = int;

	template<typename Key>
		requires requires(Key k1, Key k2)
	{
		k1 == k2;
	}
	bool operator()(const Key& o1, const Key& o2)
	{
		return o1 == o2;
	}
	
	template<typename FindKey, typename StorageKey>
		requires requires(FindKey f, StorageKey s)
	{
		f == s;
	}
	bool operator()(const FindKey& o1, const StorageKey& o2) const
	{
		return o1 == o2;
	}
};
