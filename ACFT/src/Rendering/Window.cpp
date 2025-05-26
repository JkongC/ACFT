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
	Ref<Window> Window::InitWindow(int width, int height, bool customized_border, UserAreaRect user_area)
	{		
		Ref<Window> window;
		
		switch (Config::GetRenderAPI())
		{
		case RenderAPI::OpenGL:
			window = MakeRef<OpenGLWindow>(width, height, customized_border, user_area);
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
					wd->m_Width = info.width;
					wd->m_Height = info.height;
				}
			};
		EventManager::Global().Subscribe(window, Events::WINDOW_RESIZE, resize_callback);

		return window;
	}

	void Window::SetUserArea(UserAreaRect area)
	{
		if (m_CustomizedBorder)
		{
			m_UserArea = area;
		}
	}
}