export module TemplateHelper.Type;

import <type_traits>;
import <concepts>;

namespace TemplateHelper::Type
{
	export struct ArgFlag {};

	export constexpr ArgFlag arg_flag_v{};

	//About template primary
	//----------------------------------------------------------------------------------
	template<typename T, template<typename...> typename Primary>
	struct _IsPrimary
	{
		static constexpr bool value = false;
	};

	template<template<typename...> typename T, template<typename...> typename Primary, typename... Args>
	struct _IsPrimary<T<Args...>, Primary>
	{
		static constexpr bool value = std::same_as<T<Args...>, Primary<Args...>>;
	};

	template<typename T, template<typename...> typename Primary>
	constexpr bool _IsPrimary_v = _IsPrimary<T, Primary>::value;

	template<typename T1, typename T2>
	struct _SamePrimary
	{
		static constexpr bool value = false;
	};

	template<template<typename...> typename T1, typename... Args1, template<typename...> typename T2, typename... Args2>
	struct _SamePrimary<T1<Args1...>, T2<Args2...>>
	{
		static constexpr bool value = std::same_as<T1<Args1...>, T2<Args1...>>;
	};

	template<typename T1, typename T2>
	constexpr bool _SamePrimary_v = _SamePrimary<T1, T2>::value;

	export template<typename T, template<typename...> typename Primary>
		concept SpecializationOf = _IsPrimary_v<std::remove_all_extents_t<T>, Primary>;

	export template<typename T1, typename T2>
		concept OfSamePrimary = _SamePrimary_v<T1, T2>;

	export template<typename T, template<typename...> typename Primary>
		constexpr bool IsSpecializationOf()
	{
		return SpecializationOf<T, Primary>;
	}

	export template<typename T1, typename T2>
		constexpr bool IsOfSamePrimary()
	{
		return OfSamePrimary<T1, T2>;
	}

	//About struct
	//------------------------------------------------------------------------
	export template<typename T1, typename... Ts>
		struct Complex
	{
		T1 o;
		Complex<Ts...> c;
	};

	export template<typename T>
		struct Complex<T>
	{
		T o;
	};

	/**
	 * Given a specific Complex type, calculate the offset of its element.
	 *
	 * @tparam Comp A specific Complex type.
	 * @tparam index The element index, starts from 1.
	 */
	export template<SpecializationOf<Complex> Comp, int index>
		constexpr std::ptrdiff_t IndexedOffsetOf()
	{
		static_assert(index >= 1, "Index must at least be 1.");

		if constexpr (index == 1)
			return 0;
		else
		{
			static_assert(requires(Comp complex) { complex.c; }, "Index exceeded.");
			return offsetof(Comp, Comp::c) + IndexedOffsetOf<decltype(Comp::c), index - 1>();
		}
	}

	/**
	 * Given a specific Complex instance, get its element.
	 *
	 * @param complex The Complex instance.
	 * @tparam index The element index, starts from 1.
	 */
	export template<int index, SpecializationOf<Complex> Comp>
		constexpr decltype(auto) IndexedGetMember(Comp& complex)
	{
		static_assert(index >= 1, "Index must at least be 1.");

		if constexpr (index == 1)
			return complex.o;
		else
		{
			static_assert(requires(Comp comp) { comp.c; }, "Index exceeded.");
			return IndexedGetMember(complex.c, index - 1);
		}
	}
}
