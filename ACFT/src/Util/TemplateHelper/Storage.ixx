export module TemplateHelper.Storage;

import <vector>;

import TemplateHelper.Type;

namespace TemplateHelper::Storage
{
	using namespace TemplateHelper::Type;

	export struct unordered
	{
	};

	export struct ordered
	{
	};

	export template<typename T>
		concept IsVector = SpecializationOf<T, std::vector>;

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
