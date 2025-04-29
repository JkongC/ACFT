module;

#include <entt/entity/registry.hpp>

module Window;

import Types;
import Config;
import Event;
import :OpenGLWindow;

namespace ACFT
{
	Ref<Window> Window::InitWindow()
	{
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