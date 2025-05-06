module;

#include <mutex>

module Renderer;

import Types;
import Log;
import Config;
import Event;
import ACFT.Thread;
import ACFT.Literals;
import ACFT.RenderingContexts;
import :OpenGLRenderer;

namespace ACFT
{
	Ref<Renderer>& Renderer::InitRenderer(Ref<Window> window)
	{
		std::lock_guard<std::mutex> lock(s_Mtx);
		
		if (s_Instance)
			return s_Instance;
		
		switch (Config::GetRenderAPI())
		{
		default:
			ACFT_LOG_ERROR("[Renderer] Trying to use an invalid Render API type!");
		case RenderAPI::OpenGL:
			s_Instance = MakeRef<OpenGLRenderer>();
			s_Instance->m_Window = window;
			break;
		}

		auto resize_callback = [&renderer = s_Instance](Ref<Event> event) -> void
			{
				if (event->GetType() == Events::WINDOW_RESIZE)
				{
					renderer->m_EventQueue.Push(event);
				}
			};
		EventManager::Global().Subscribe(s_Instance, Events::WINDOW_RESIZE, resize_callback);

		return s_Instance;
	}

	Ref<Renderer>& Renderer::GetRenderer()
	{
		std::lock_guard<std::mutex> lock(s_Mtx);

		if (!s_Instance)
		{
			ACFT_LOG_FATAL("[Renderer] Error occurred: Trying to get the renderer without initializing it.");
			exit(-1);
		}

		return s_Instance;
	}

	Ref<Window> Renderer::GetWindow()
	{
		return m_Window;
	}
}