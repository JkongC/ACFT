#include "stdafx.h"

#include "Timer.h"
#include "Log/Logger.h"

namespace ACFT
{
	NormalTimer::NormalTimer()
	{
		this->last = std::chrono::high_resolution_clock::now();
	}

	int NormalTimer::GetElapsed()
	{
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<int, std::milli> elapsed = std::chrono::duration_cast<std::chrono::duration<int, std::milli>>(now - this->last);

		this->last = now;
		return elapsed.count();
	}


	ScopedTimer::ScopedTimer()
	{
		this->start = std::chrono::high_resolution_clock::now();
	}

	ScopedTimer::~ScopedTimer()
	{
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<int, std::milli> elapsed = std::chrono::duration_cast<std::chrono::duration<int, std::milli>>(now - this->start);
		ACFT_LOG_INFO("Time elapsed: {} ms.", elapsed.count());
	}
}