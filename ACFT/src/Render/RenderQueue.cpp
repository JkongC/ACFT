#include "stdafx.h"

#include "ACFT.h"
#include "RenderQueue.h"

namespace ACFT
{
	RenderQueue::RenderQueue()
		: command_queue()
	{

	}
	
	void RenderQueue::PushCommand(Ref<RenderCommand> command)
	{
		command_queue.Push(command);
	}

	std::optional<Ref<RenderCommand>> RenderQueue::FetchCommand()
	{
		return command_queue.Pop();
	}
}