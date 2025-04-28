export module FPSProfiler;

import Timer;

namespace ACFT
{
	export class FPSProfiler
	{
	public:
		FPSProfiler() = default;
		FPSProfiler(float internal) : m_Interval(internal) {}

		void RecordFrame();

	private:
		float m_TimeCounter = 0;
		float m_FrameCounter = 0;
		float m_Interval = 1000.0f;
		NormalTimer<float, ACFT::millisecond> m_Timer;
	};
}
