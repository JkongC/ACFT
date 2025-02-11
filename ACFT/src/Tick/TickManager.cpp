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
		logic_tick_accumulator += this->logic_timer.GetElapsed();
		int count_of_ticks = 0;

		while (logic_tick_accumulator > LogicMSPT)
		{
			logic_tick_accumulator -= LogicMSPT;
			count_of_ticks++;
		}

		for (; count_of_ticks > 0; count_of_ticks--)
		{
			for (auto& ticker : this->logic_tickers)
			{
				ticker->TickLogic(LogicMSPT);
			}
		}
	}

	void TickManager::TickAction()
	{

	}

	float TickManager::GetTimeDelta()
	{
		return logic_tick_accumulator / LogicMSPT;
	}

	void TickManager::AddLogicTicker(LogicTicker* ticker)
	{
		this->logic_tickers.insert(ticker);
	}

	void TickManager::AddActionTicker(ActionTicker* ticker)
	{
	}
}