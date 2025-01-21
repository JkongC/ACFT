#include "stdafx.h"

#include "ACFT.h"
#include "EventManager.h"

namespace ACFT
{
	void EventListener::EventSubscribe(Event::Type type)
	{
		EventManager::GetInstance().Subscribe(this, type);
	}

	void EventManager::Subscribe(EventListener* listener, Event::Type type)
	{
		this->subscribers[type].insert(listener);
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