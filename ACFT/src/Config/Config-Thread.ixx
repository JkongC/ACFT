export module Config.Thread;

#if defined (ACFT_RENDER_THREAD)
constexpr bool USE_RENDER_THREAD = true;
#else
constexpr bool USE_RENDER_THREAD = false;
#endif

#if defined (ACFT_EVENT_THREAD)
constexpr bool USE_EVENT_THREAD = true;
#else
constexpr bool USE_EVENT_THREAD = false;
#endif

namespace ACFT::Config
{
	namespace CompileTime
	{
		export constexpr bool IsRenderThreadUsed()
		{
			return USE_RENDER_THREAD;
		}

		export constexpr bool IsEventThreadUsed()
		{
			return USE_EVENT_THREAD;
		}

		export constexpr int GetFunctionalThreadCount()
		{
			return static_cast<int>(IsRenderThreadUsed()) + static_cast<int>(IsEventThreadUsed());
		}
	}
}
