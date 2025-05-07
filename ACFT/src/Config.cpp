module Config;

import Image;
import Renderer;

namespace ACFT::Config
{
	static inline RenderAPI RENDER_API = RenderAPI::OpenGL;
	static inline std::filesystem::path TEXTURE_PATH = "textures";
	static inline Image WINDOW_ICON{};
	static inline std::string WINDOW_NAME = "AnotherCraft";
	static inline bool USE_RENDER_THREAD = true;
	static inline bool USE_FPS_PROFILER = false;
	
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

	void UseRenderThread(bool use)
	{
		USE_RENDER_THREAD = use;
	}

	bool IsRenderThreadUsed()
	{
		return USE_RENDER_THREAD;
	}

	void UseFPSProfiler(bool use)
	{
		USE_FPS_PROFILER = use;
	}

	bool IsFPSProfilerUsed()
	{
		return USE_FPS_PROFILER;
	}
}