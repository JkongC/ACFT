export module FPSProfiler;

import Timer;

namespace ACFT
{
	export class FPSProfiler
	{
	public:
		static void SetInterval(float interval) { s_Interval = interval; }
		static float GetInterval() { return s_Interval; }
		
		static void RecordFrame();

	private:
		FPSProfiler() = default;
		FPSProfiler(const FPSProfiler&) = delete;
		FPSProfiler(FPSProfiler&&) = delete;
		FPSProfiler& operator=(const FPSProfiler&) = delete;

	private:
		static inline float s_TimeCounter = 0;
		static inline float s_FrameCounter = 0;
		static inline float s_Interval = 1000.0f;
		static inline NormalTimer<float, ACFT::millisecond> m_Timer;
	};
}
