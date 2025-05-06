export module ACFT.RenderingContexts;

namespace ACFT
{
	export namespace ThreadFeatures
	{
		thread_local bool is_render_thread = false;
	}
}
