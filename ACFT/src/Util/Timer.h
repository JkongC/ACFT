#ifndef ACFT_TIMER_H_
#define ACFT_TIMER_H_

namespace ACFT
{
	using nanosecond = std::nano;
	using millisecond = std::milli;
	using second = std::ratio<1, 1>;

	template<typename type, typename period>
	concept Duration_Convertible = std::is_arithmetic_v<type> && std::_Is_ratio_v<period>;

	class NormalTimer
	{
	public:
		NormalTimer();
		~NormalTimer() = default;

		int GetElapsed();
		int GetElapsedAndFlush();
		void Flush();

		void Decline(int decline);

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> last;
	};

	template<typename type, typename period>
		requires Duration_Convertible<type, period>
	class ScopedTimer
	{
	public:
		ScopedTimer() = delete;

		ScopedTimer(std::function<void(type)> _callback)
			: callback(_callback)
		{
			this->start = std::chrono::high_resolution_clock::now();
		}

		~ScopedTimer()
		{
			auto now = std::chrono::high_resolution_clock::now();
			std::chrono::duration<type, period> elapsed = std::chrono::duration_cast<std::chrono::duration<type, period>>(now - this->start);
			this->callback(elapsed.count());
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
		std::function<void(type)> callback;
	};
}

#endif