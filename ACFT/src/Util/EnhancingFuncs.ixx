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
		
		export template<typename T>
		concept IsVector = TemplateHelper::SpecializationOf<T, std::vector>;

		/**
		 * Remove an element, regardless of the total order of all elements.
		 * 
		 * \tparam Storage The container type, supports only std::vector for now.
		 * \param storage The container.
		 * \param it The iterator pointing to the element to be removed.
		 * \return Whether this function succeeds.
		 */
		export template<typename Storage>
			requires IsVector<Storage>
		bool UnorderedRemove(Storage& storage, typename Storage::iterator it)
		{
			if (it == storage.end())
				return false;

			std::swap(*it, storage.back());
			storage.pop_back();

			return true;
		}

		export template<typename Storage, typename Order_Type = unordered>
			requires requires{ typename Storage::iterator; } &&
		(std::is_same_v<Order_Type, ordered> || std::is_same_v<Order_Type, unordered>)
		bool Remove(Storage& storage, typename Storage::iterator it)
		{
			if constexpr (std::is_same_v<Order_Type, ordered>)
			{
				static_assert(false && sizeof(Order_Type), "Ordered remove is not implemented.");
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
		T& Value(Var&& variant) noexcept
		{
			return std::get<T>(variant);
		}
	}
}
