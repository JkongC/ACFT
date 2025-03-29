module;

#include <random>

module UUID;

std::random_device random_device;
std::mt19937_64 rd_engine(random_device());
std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

ACFT::UUID::UUID()
	: internal_id(dist(rd_engine))
{
}

ACFT::UUID::UUID(const UUID& uuid)
	: internal_id(uuid.internal_id)
{
}