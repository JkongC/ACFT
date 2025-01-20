#ifndef ACFT_EVENT_H_
#define ACFT_EVENT_H_

namespace ACFT
{
	class Event
	{

	};

	class EventListener
	{
	public:
		virtual void HandleEvent(Event event) = 0;
	};

	class InputEvent : Event
	{

	};
}

#endif