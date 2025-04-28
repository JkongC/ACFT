module Config;

import Image;
import Renderer;

namespace ACFT::Config
{
	static inline RenderAPI RENDER_API = RenderAPI::OpenGL;
	static inline std::filesystem::path TEXTURE_PATH = "textures";
	static inline Image WINDOW_ICON{};
	static inline std::string WINDOW_NAME = "AnotherCraft";
	
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

	void SetWindowIcon(std::filesystem::path icon)
	{
		WINDOW_ICON.InitImage(icon);
	}

	Image& GetWindowIcon()
	{
		return WINDOW_ICON;
	}

	void SetWindowName(std::string name)
	{
		WINDOW_NAME = name;
	}

	std::string_view GetWindowName()
	{
		return WINDOW_NAME;
	}
}