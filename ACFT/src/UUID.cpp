module;

#include <random>
#include <chrono>

module UUID;

static std::random_device random_device;
static std::mt19937_64 rd_engine(random_device());
static std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);

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

	UUID::UUID(UUID&& other) noexcept
		: pack(other.pack)
	{
		other.pack.high = 0;
	}

	bool UUID::operator==(const UUID& other) const
	{
		return (pack.low == other.pack.low) && (pack.high == other.pack.high);
	}

	bool UUID::operator!=(const UUID& other) const
	{
		return (pack.low != other.pack.low) || (pack.high != other.pack.high);
	}

	UUID& UUID::operator=(const UUID& other) noexcept
	{
		pack = other.pack;
		return *this;
	}

	UUID& UUID::operator=(UUID&& other) noexcept
	{
		pack = other.pack;
		other.pack.high = 0;
		return *this;
	}

	UUID::operator bool() const noexcept
	{
		return pack.high != 0;
	}
}