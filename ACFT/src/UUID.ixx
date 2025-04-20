module;

#include "Platform.h"

export module UUID;

import <cstdint>;
import <functional>;

namespace ACFT
{
	export class ACFT_API UUID
	{
	public:
		UUID();
		UUID(const UUID&);

		inline uint64_t GetID() const { return internal_id; }

		bool operator==(const UUID& other) const { return this->internal_id == other.internal_id; }
		bool operator!=(const UUID& other) const { return this->internal_id != other.internal_id; }

	private:
		uint64_t internal_id;
	};
}

namespace std
{
	template<>
	struct hash<ACFT::UUID>
	{
		size_t operator()(const ACFT::UUID& uuid) const
		{
			return std::hash<uint64_t>()(uuid.GetID());
		}
	};
}
