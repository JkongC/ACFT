export module ACFT.RenderingContexts;

import <mutex>;

namespace ACFT
{
	namespace ThreadFeatures
	{
		std::mutex state_lock;
		
		thread_local bool is_render_thread = false;
		bool render_thread_set = false;

		export void SetRenderThread()
		{
			std::lock_guard<std::mutex> lock(state_lock);
			if (!render_thread_set)
			{
				is_render_thread = true;
				render_thread_set = true;
			}
		}

		export bool IsRenderThread()
		{
			return is_render_thread;
		}

	}
}
