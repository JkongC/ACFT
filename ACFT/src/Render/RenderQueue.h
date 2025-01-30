#ifndef ACFT_RENDERQUEUE_H_
#define ACFT_RENDERQUEUE_H_

#include "RenderCommand.h"

namespace ACFT
{
	class RenderQueue
	{
	public:
		void AddCommand(const RenderCommand& command);
		const RenderCommand& FetchCommand();

	private:
		std::deque<RenderCommand> command_queue;
		std::mutex mtx;

	};
}

#endif
