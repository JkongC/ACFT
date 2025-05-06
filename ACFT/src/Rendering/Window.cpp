module;

#include <entt/entity/registry.hpp>

module Window;

import Types;
import Config;
import Event;
import Log;
import ACFT.Thread;
import ACFT.Literals;
import ACFT.RenderingContexts;
import :OpenGLWindow;

namespace ACFT
{
	Ref<Window> Window::InitWindow()
	{
		if (Config::IsRenderThreadUsed() && !ThreadManager::GetThread(Threads::RENDER_THREAD)->IsCurrentThread())
		{
			ACFT_LOG_ERROR("Window can only be initialized in render thread!");
			return nullptr;
		}
		else
		{
			ThreadFeatures::is_render_thread = true;
		}
			
		Ref<Window> window;
		
		switch (Config::GetRenderAPI())
		{
		case RenderAPI::OpenGL:
			window = MakeRef<OpenGLWindow>();
			break;
		default:
			window = nullptr;
			break;
		}

		auto resize_callback = [wd = window.get()](Ref<Event> event) -> void 
			{
				if (event->GetType() == Events::WINDOW_RESIZE)
				{
					auto info = *event->GetInfo<WindowSizeInfo>();
					wd->SetWidth(info.width);
					wd->SetHeight(info.height);
				}
			};
		EventManager::Global().Subscribe(window, Events::WINDOW_RESIZE, resize_callback);

		return window;
	}
}