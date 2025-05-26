module Config;

import Image;
import Renderer;

namespace ACFT::Config
{
	static inline bool CONFIG_LOCKED = false;
	
	static inline RenderAPI RENDER_API = RenderAPI::OpenGL;

	static inline std::filesystem::path TEXTURE_PATH = "textures";
	static inline std::filesystem::path SHADER_PATH = "resources/shaders";

	static inline Image WINDOW_ICON{};
	static inline std::string WINDOW_NAME = "AnotherCraft";
	static inline int WINDOW_WIDTH = 1440;
	static inline int WINDOW_HEIGHT = 960;
	static inline UserAreaRect WINDOW_USER_AREA{};

	static inline bool USE_FPS_PROFILER = false;
	
	void LockConfig()
	{
		CONFIG_LOCKED = true;
	}
	
	void SetRenderAPI(RenderAPI render_API)
	{
		if (!CONFIG_LOCKED) 
		{
			RENDER_API = render_API;
		}
	}

	RenderAPI GetRenderAPI()
	{
		return RENDER_API;
	}

	void SetTexturePath(std::filesystem::path path)
	{
		if (!CONFIG_LOCKED)
		{
			TEXTURE_PATH = path;
		}
	}

	std::filesystem::path GetTexturePath()
	{
		return TEXTURE_PATH;
	}

	void SetShaderPath(std::filesystem::path path)
	{
		if (!CONFIG_LOCKED) 
		{
			SHADER_PATH = path;
		}
	}

	std::filesystem::path GetShaderPath()
	{
		return SHADER_PATH;
	}

	void SetWindowIcon(std::filesystem::path icon)
	{
		if (!CONFIG_LOCKED) 
		{
			WINDOW_ICON.InitImage(icon);
		}
	}

	Image& GetWindowIcon()
	{
		return WINDOW_ICON;
	}

	void SetWindowName(std::string name)
	{
		if (!CONFIG_LOCKED) 
		{
			WINDOW_NAME = name;
		}
	}

	std::string_view GetWindowName()
	{
		return WINDOW_NAME;
	}

	void SetWindowSize(int width, int height)
	{
		if (!CONFIG_LOCKED) 
		{
			WINDOW_WIDTH = width;
			WINDOW_HEIGHT = height;
		}
	}

	std::pair<int, int> GetWindowSize()
	{
		return { WINDOW_WIDTH, WINDOW_HEIGHT };
	}

	void SetWindowUserArea(UserAreaRect user_area)
	{
		if (!CONFIG_LOCKED)
		{
			WINDOW_USER_AREA = user_area;
		}
	}

	UserAreaRect GetWindowUserArea()
	{
		return WINDOW_USER_AREA;
	}

	void UseFPSProfiler(bool use)
	{
		if (!CONFIG_LOCKED) 
		{
			USE_FPS_PROFILER = use;
		}
	}

	bool IsFPSProfilerUsed()
	{
		return USE_FPS_PROFILER;
	}
}