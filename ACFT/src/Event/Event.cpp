#include "stdafx.h"

#include "ACFT.h"
#include "Event.h"

namespace ACFT
{
	Event::Event(Type type)
		: type(type)
	{

	}

	InputEvent::InputEvent(Type type)
		: Event(type)
	{
	}
}