#include "stdafx.h"

#include "Timer.h"
#include "Log/Logger.h"

namespace ACFT
{
	NormalTimer::NormalTimer()
	{
		this->last = std::chrono::high_resolution_clock::now();
	}

	float NormalTimer::GetElapsed()
	{
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float, std::milli> elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(now - this->last);

		this->last = now;
		return elapsed.count();
	}

	float NormalTimer::GetElapsedAndFlush()
	{
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float, std::milli> elapsed = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(now - this->last);

		this->last = now;
		return elapsed.count();
	}

	void NormalTimer::Flush()
	{
		this->last = std::chrono::high_resolution_clock::now();
	}

	void NormalTimer::Decline(float decline)
	{
		//std::chrono::duration<float, std::milli> should_decline(decline);
		//this->last += should_decline;
	}
}