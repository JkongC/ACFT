module;

#include <random>
#include <chrono>

module UUID;

std::random_device random_device;
std::mt19937_64 rd_engine(random_device());
std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

namespace ACFT
{
	UUID::UUID()
	{
		pack.low = dist(rd_engine);

		auto now = std::chrono::system_clock::now();
		auto ts = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

		pack.high = std::hash<decltype(ts)>()(ts);
	}

	UUID::UUID(const UUID& other)
		: pack(other.pack)
	{ }

	bool UUID::operator==(const UUID& other) const
	{
		return (pack.low == other.pack.low) && (pack.high == other.pack.high);
	}

	bool UUID::operator!=(const UUID& other) const
	{
		return (pack.low != other.pack.low) || (pack.high != other.pack.high);
	}
}