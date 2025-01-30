#include "stdafx.h"

#include "ACFT.h"
#include "RenderQueue.h"

namespace ACFT
{
	void RenderQueue::AddCommand(const RenderCommand& command)
	{
	}

	const RenderCommand& RenderQueue::FetchCommand()
	{
		return *(RenderCommand*)0;
	}
}