module Config;

import Renderer;

namespace ACFT::Config
{
	static inline RenderAPI RENDER_API = RenderAPI::OpenGL;
	static inline std::filesystem::path TEXTURE_PATH = "textures";
	
	void SetRenderAPI(RenderAPI render_API)
	{
		RENDER_API = render_API;
	}

	RenderAPI GetRenderAPI()
	{
		return RENDER_API;
	}

	void SetTexturePath(std::filesystem::path path)
	{
		TEXTURE_PATH = path;
	}

	std::filesystem::path GetTexturePath()
	{
		return TEXTURE_PATH;
	}
}