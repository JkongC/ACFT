module;

#include "Platform.h"

export module Config;

import <filesystem>;
import Renderer;

namespace ACFT::Config
{	
	export ACFT_API void SetRenderAPI(RenderAPI render_API);
	export ACFT_API RenderAPI GetRenderAPI();

	export ACFT_API void SetTexturePath(std::filesystem::path path);
	export ACFT_API std::filesystem::path GetTexturePath();
}