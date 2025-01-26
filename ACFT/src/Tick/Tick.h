#ifndef ACFT_TICK_H_
#define ACFT_TICK_H_

namespace ACFT
{
	class LogicTicker
	{
	public:
		LogicTicker();

		virtual void TickLogic(float delta) = 0;
	};

	class ActionTicker
	{
	public:
		ActionTicker();

		virtual void TickAction(float delta) = 0;
	};

}

#include "TickManager.h"

#endif
