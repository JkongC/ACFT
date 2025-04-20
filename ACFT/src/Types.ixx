module;

#include <memory>

export module Types;

namespace ACFT
{
	export template<typename T>
	using Ref = std::shared_ptr<T>;

	export template<typename T>
	using Scope = std::unique_ptr<T>;

	export template<typename T>
	using View = std::weak_ptr<T>;

	export template<typename T>
	struct ViewHash
	{
		size_t operator()(const View<T>& view) const
		{
			auto v = view.lock();
			if (v)
			{
				return std::hash<void*>()(v.get());
			}

			return 0;
		}
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

	export template<typename T, typename... Args>
	Ref<T> MakeRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	export template<typename T, typename... Args>
	Scope<T> MakeScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	export template<typename T>
	View<T> MakeView(Ref<T>&& ref)
	{
		return View(ref);
	}

	export template<typename T1, typename T2>
	Ref<T1> RefCast(Ref<T2>&& ref)
	{
		return std::static_pointer_cast<T1>(ref);
	}

	export template<typename T1, typename T2>
	Scope<T1> ScopeCast(Scope<T2>&& scope)
	{
		return std::static_pointer_cast<T1>(scope);
	}
}