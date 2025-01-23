#include "stdafx.h"

#include "ACFT.h"
#include "EventManager.h"

namespace ACFT
{
	void EventListener::EventSubscribe(Event::Type type)
	{
		EventManager::GetInstance().Subscribe(this, type);
	}

	void EventListener::EventUnsubscribe(Event::Type type)
	{
		EventManager::GetInstance().Unsubscribe(this, type);
	}

	void EventManager::Subscribe(EventListener* listener, Event::Type type)
	{
		this->subscribers[type].insert(listener);
	}

	void EventManager::Unsubscribe(EventListener* listener, Event::Type type)
	{
		auto& list = this->subscribers[type];
		auto it = list.find(listener);
		if (it != list.end())
			list.erase(it);
	}

	void EventManager::Poll()
	{

	}

	void EventManager::Trigger(const Event& event)
	{
		for (auto& subscriber : subscribers[event.GetType()])
		{
			subscriber->HandleEvent(event);
		}
	}
}