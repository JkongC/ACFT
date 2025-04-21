module;

#include <mutex>

module Renderer;

import Types;
import Log;
import Config;
import :OpenGLRenderer;

namespace ACFT
{
	Renderer& Renderer::InitRenderer(Ref<Window> window)
	{
		std::lock_guard<std::mutex> lock(s_Mtx);
		
		if (s_Instance)
			return *s_Instance;
		
		switch (Config::GetRenderAPI())
		{
		default:
			ACFT_LOG_ERROR("[Renderer] Trying to use an invalid Render API type!");
		case RenderAPI::OpenGL:
			s_Instance = new OpenGLRenderer();
			s_Instance->m_Window = window;
			return *s_Instance;
			break;
		}
	}

	Renderer& Renderer::GetRenderer()
	{
		std::lock_guard<std::mutex> lock(s_Mtx);

		if (!s_Instance)
		{
			ACFT_LOG_FATAL("[Renderer] Error occurred: Trying to get the renderer without initializing it.");
			exit(-1);
		}

		return *s_Instance;
	}

	void Renderer::ShutdownRenderer()
	{
		std::lock_guard<std::mutex> lock(s_Mtx);

		if (s_Instance)
			delete s_Instance;
	}
}