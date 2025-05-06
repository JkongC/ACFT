module;

#include "Platform.h"

export module Config;

import <filesystem>;
import Image;
import Renderer;

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

	export ACFT_API void UseRenderThread(bool use);
	export ACFT_API bool IsRenderThreadUsed();
}