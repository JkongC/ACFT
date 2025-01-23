#ifndef ACFT_TICKMANAGER_H_
#define ACFT_TICKMANAGER_H_

#include "Util/Timer.h"

namespace ACFT
{
	constexpr float LogicTPS = 180;
	constexpr float ActionTPS = 20;
	constexpr float LogicMSPT = 1000 / LogicTPS;
	constexpr float ActionMSPT = 1000 / ActionTPS;
	
	class TickManager
	{
	public:
		inline static TickManager& GetInstance()
		{
			static TickManager manager;
			return manager;
		}

		void TickLogic();

		void TickAction();

		float GetTimeDelta();

		void AddLogicTicker(LogicTicker* ticker);

		void AddActionTicker(ActionTicker* ticker);

	private:
		TickManager();
	
	private:
		NormalTimer logic_timer;
		NormalTimer action_timer;

		std::unordered_set<LogicTicker*> logic_tickers;
	};
}

#endif