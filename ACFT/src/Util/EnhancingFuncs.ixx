export module ACFT.EnhancingFuncs;

import <vector>;

namespace ACFT
{
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
			bool operator()(Storage& storage, typename Storage::iterator it)
			{
				return false;
			}
			
			static_assert(false, "This kind of storage is not supported.");
		};

		export template<typename T>
		struct UnorderedRemove<std::vector<T>>
		{
			bool operator()(std::vector<T>& storage, std::vector<T>::iterator it)
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
				return UnorderedRemove<Storage>()(storage, it);
			}
		}
	}
}
