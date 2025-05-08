module;

#include "Platform.h"

export module Config;

import <filesystem>;
import Image;
import Renderer;

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
	export ACFT_API void SetRenderAPI(RenderAPI render_API);
	export ACFT_API RenderAPI GetRenderAPI();

	export ACFT_API void SetTexturePath(std::filesystem::path path);
	export ACFT_API std::filesystem::path GetTexturePath();

	export ACFT_API void SetWindowIcon(std::filesystem::path icon);
	export ACFT_API Image& GetWindowIcon();

	export ACFT_API void SetWindowName(std::string name);
	export ACFT_API std::string_view GetWindowName();
	
	export ACFT_API void UseFPSProfiler(bool use);
	export ACFT_API bool IsFPSProfilerUsed();

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
	}
}