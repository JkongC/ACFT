module FPSProfiler;

import Log;

namespace ACFT
{
	void FPSProfiler::RecordFrame()
	{
		s_TimeCounter += m_Timer.GetElapsed();
		s_FrameCounter += 1;

		m_Timer.Flush();

		if (s_TimeCounter >= s_Interval)
		{
			ACFT_LOG_INFO("Current FPS: {}", static_cast<size_t>(s_FrameCounter / (s_TimeCounter / 1000.0f)));
			s_FrameCounter = 0;
			s_TimeCounter = 0;
		}
	}
}
