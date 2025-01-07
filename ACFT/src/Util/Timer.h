#ifndef ACFT_TIMER_H_
#define ACFT_TIMER_H_

namespace ACFT 
{
	class NormalTimer
	{
	public:
		NormalTimer();
		~NormalTimer() = default;

		int GetElapsed();
	
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> last;
	};

	class ScopedTimer
	{
	public:
		ScopedTimer();
		~ScopedTimer();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
	};
}

#endif