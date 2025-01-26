#include "stdafx.h"

#include "ACFT.h"
#include "Tick.h"

namespace ACFT
{
	LogicTicker::LogicTicker()
	{
		TickManager::GetInstance().AddLogicTicker(this);
	}

	ActionTicker::ActionTicker()
	{
		TickManager::GetInstance().AddActionTicker(this);
	}
}