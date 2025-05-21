module;

#include "Platform.h"

export module UUID;

import <cstdint>;
import <functional>;
import <string_view>;

namespace ACFT
{
	export class ACFT_API UUID
	{
	public:
		UUID();
		UUID(const UUID&);

		bool operator==(const UUID& other) const;
		bool operator!=(const UUID& other) const;

	private:
		friend struct std::hash<UUID>;
		struct
		{
			uint64_t low;
			uint64_t high;
		} pack;
	};
}

namespace std
{
	template<>
	struct hash<ACFT::UUID>
	{
		size_t operator()(const ACFT::UUID& uuid) const
		{
			std::string_view v{reinterpret_cast<const char*>(&uuid.pack), sizeof(uuid.pack)};
			return std::hash<std::string_view>()(v);
		}
	};
}
