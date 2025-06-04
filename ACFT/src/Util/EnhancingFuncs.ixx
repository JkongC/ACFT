export module ACFT.EnhancingFuncs;

import <vector>;
import <variant>;
import <functional>;

namespace ACFT
{
	namespace TemplateHelper
	{
		export struct ArgFlag {};

		export constexpr ArgFlag arg_flag_v{};
		
		template<typename T>
		struct primary
		{
			using type = void;
		};

		template<typename R>
		struct primary<R&> : primary<R> {};

		template<typename R>
		struct primary<R&&> : primary<R> {};

		template<template<typename> typename T, typename... Args>
		struct primary<T<Args...>>
		{
			using type = std::decay_t<T<void>>;
		};

		template<typename T>
		using primary_t = primary<T>::type;

		export template<typename T, template<typename> typename Primary>
		concept SpecializationOf = std::is_same_v<primary_t<T>, Primary<void>>;

		export template<typename T1, typename T2>
		concept OfSamePrimary = std::is_same_v<primary_t<T1>, primary_t<T2>>;

		export template<typename T, template<typename> typename Primary>
		constexpr bool IsSpecializationOf()
		{
			return SpecializationOf<T, Primary>;
		}

		export template<typename T1, typename T2>
		constexpr bool IsOfSamePrimary()
		{
			return OfSamePrimary<T1, T2>;
		}
	}
	
	namespace StorageHelper
	{
		export struct unordered
		{ };

		export struct ordered
		{ };
		
		export template<typename Storage>
			requires requires{ typename Storage::value_type; }
		struct UnorderedRemove
		{
			static bool operator()(Storage& storage, typename Storage::iterator it)
			{
				return false;
			}
			
			static_assert(false, "This kind of storage is not supported.");
		};

		export template<typename T>
		struct UnorderedRemove<std::vector<T>>
		{
			static bool operator()(std::vector<T>& storage, std::vector<T>::iterator it)
			{
				if (it == storage.end())
					return false;
				
				T& del_item = *it;
				std::swap(del_item, storage.back());
				storage.pop_back();

				return true;
			}
		};

		export template<typename Storage, typename Order_Type = unordered>
			requires requires{ typename Storage::iterator; } &&
		(std::is_same_v<Order_Type, ordered> || std::is_same_v<Order_Type, unordered>)
		bool Remove(Storage& storage, typename Storage::iterator it)
		{
			if constexpr (std::is_same_v<Order_Type, ordered>)
			{
				static_assert(false, "Ordered remove is not implemented.");
				return false;
			}
			else
			{
				return UnorderedRemove<Storage>(storage, it);
			}
		}
	}

	namespace VariantHelper
	{
		using namespace TemplateHelper;
		
		export template<typename T, SpecializationOf<std::variant> Var>
		bool IsType(Var&& variant)
		{
			return std::get_if<T>(variant) != nullptr;
		}
		
		export template<typename T, SpecializationOf<std::variant> Var>
		T ValueOr(Var&& variant, const T& alternative)
		{
			T* result = std::get_if<T>(&variant);
			if (result)
				return *result;
			else
				return alternative;
		}

		export template<typename T, SpecializationOf<std::variant> Var>
		T ValueOrDo(Var&& variant, const T& alternative, std::function<void()> reaction)
		{
			T* result = std::get_if<T>(&variant);
			if (result)
				return *result;
			else
			{
				reaction();
				return alternative;
			}
		}

		export template<typename T, SpecializationOf<std::variant> Var>
		T Value(Var&& variant)
		{
			return ValueOr<T>(std::forward<Var>(variant), T{});
		}
	}
}
