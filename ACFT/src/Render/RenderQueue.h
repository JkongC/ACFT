#ifndef ACFT_RENDERQUEUE_H_
#define ACFT_RENDERQUEUE_H_

#include "RenderCommand.h"
#include "Util/LockfreeQueue.h"

namespace ACFT
{
	class RenderQueue
	{
	public:
		RenderQueue();
		
		void PushCommand(Ref<RenderCommand> command);
		std::optional<Ref<RenderCommand>> FetchCommand();

	private:
		LockfreeQueue<RenderCommand> command_queue;

	};
}

#endif
