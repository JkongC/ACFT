module FPSProfiler;

import Log;

namespace ACFT
{
	void FPSProfiler::RecordFrame()
	{
		m_TimeCounter += m_Timer.GetElapsed();
		m_FrameCounter += 1;

		m_Timer.Flush();

		if (m_TimeCounter >= m_Interval)
		{
			ACFT_LOG_INFO("Current FPS: {}", static_cast<size_t>(m_FrameCounter / (m_TimeCounter / 1000.0f)));
			m_FrameCounter = 0;
			m_TimeCounter = 0;
		}
	}
}
