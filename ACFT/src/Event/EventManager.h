#ifndef ACFT_EVENTMANAGER_H_
#define ACFT_EVENTMANAGER_H_

#include "Event.h"

namespace ACFT
{
	class EventListener
	{
	public:
		virtual void HandleEvent(const Event& event) = 0;
		void EventSubscribe(Event::Type type);
		void EventUnsubscribe(Event::Type type);

	protected:
		EventListener() = default;
		EventListener(const EventListener&) = default;
		EventListener(EventListener&&) = default;
	};

	class EventManager
	{
	public:
		static EventManager& GetInstance()
		{
			static EventManager manager;
			return manager;
		}

		void Subscribe(EventListener* listener, Event::Type type);
		void Unsubscribe(EventListener* listener, Event::Type type);

		void Poll();
		void Trigger(const Event& event);

	private:
		EventManager() = default;
		EventManager(const EventManager&) = delete;
		EventManager(EventManager&&) = delete;

	private:
		std::unordered_map<Event::Type, std::unordered_set<EventListener*>> subscribers;
	};
}

#endif

