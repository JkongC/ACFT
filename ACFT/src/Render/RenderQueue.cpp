#include "stdafx.h"

#include "ACFT.h"
#include "RenderQueue.h"

namespace ACFT
{
	RenderQueue::RenderQueue()
		: command_queue()
	{

	}
	
	void RenderQueue::PushCommand(Scope<RenderCommand>&& command)
	{
		command_queue.Push(std::move(command));
	}

	std::optional<Scope<RenderCommand>> RenderQueue::FetchCommand()
	{
		return std::move(command_queue.Pop());
	}
}