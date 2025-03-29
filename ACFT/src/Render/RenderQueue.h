#ifndef ACFT_RENDERQUEUE_H_
#define ACFT_RENDERQUEUE_H_

#include "RenderCommand.h"
import LockfreeQueue;

namespace ACFT
{
	class RenderQueue
	{
	public:
		RenderQueue();
		
		void PushCommand(Scope<RenderCommand>&& command);
		std::optional<Scope<RenderCommand>> FetchCommand();

	private:
		LockfreeQueue<RenderCommand, Scope> command_queue;

	};
}

#endif
