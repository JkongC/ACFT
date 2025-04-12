export module Config;

import <filesystem>;
import Renderer;

namespace ACFT::Config
{
	export void SetRenderAPI(RenderAPI render_API);
	export RenderAPI GetRenderAPI();

	export void SetTexturePath(std::filesystem::path path);
	export std::filesystem::path GetTexturePath();
}