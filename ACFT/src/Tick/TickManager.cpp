#include "stdafx.h"

#include "ACFT.h"
#include "Tick.h"

namespace ACFT
{
	TickManager::TickManager() 
		: logic_timer(), action_timer()
		, logic_tickers()
	{

	}
	
	void TickManager::TickLogic()
	{
		float elapsed = this->logic_timer.GetElapsed();
		int count_of_ticks = 0;

		while (elapsed > LogicMSPT)
		{
			elapsed -= LogicMSPT;
			count_of_ticks++;
		}

		this->logic_timer.Decline(count_of_ticks * LogicMSPT);

		for (; count_of_ticks > 0; count_of_ticks--)
		{
			for (auto& ticker : this->logic_tickers)
			{
				ticker->TickLogic(elapsed);
			}
		}
	}

	void TickManager::TickAction()
	{

	}

	float TickManager::GetTimeDelta()
	{
		float elapsed = logic_timer.GetElapsed();
		return elapsed / LogicMSPT;
	}

	void TickManager::AddLogicTicker(LogicTicker* ticker)
	{
		this->logic_tickers.insert(ticker);
	}

	void TickManager::AddActionTicker(ActionTicker* ticker)
	{
	}
}