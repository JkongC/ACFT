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

	std::pair<float, float> Window::GetUserAreaSize()
	{
		if (m_UserArea.use_percentage)
		{
			return { m_Width * (1.0f - m_UserArea.left - m_UserArea.right), m_Height * (1.0f - m_UserArea.top - m_UserArea.bottom) };
		}
		else
		{
			return { m_Width - m_UserArea.left - m_UserArea.right, m_Height - m_UserArea.top - m_UserArea.bottom };
		}
	}

	bool Window::IsPosInUserArea(double x, double y)
	{
		double left, right, top, bottom;
		if (m_UserArea.use_percentage)
		{
			left = m_Width * m_UserArea.left;
			right = m_Width * (1.0f - m_UserArea.right);
			top = m_Height * m_UserArea.top;
			bottom = m_Height * (1.0f - m_UserArea.bottom);
		}
		else
		{
			left = m_UserArea.left;
			right = m_Width - m_UserArea.right;
			top = m_UserArea.top;
			bottom = m_Height - m_UserArea.bottom;
		}
		return x >= left && x <= right && y >= top && y <= bottom;
	}

	std::pair<double, double> Window::WindowPosToUserAreaPos(double x, double y)
	{
		double O_x, O_y;

		if (m_UserArea.use_percentage)
		{
			O_x = m_Width * m_UserArea.left;
			O_y = m_Height * m_UserArea.top;
		}
		else
		{
			O_x = m_UserArea.left;
			O_y = m_UserArea.top;
		}

		return { x - O_x, y - O_y };
	}

	std::pair<double, double> Window::UserAreaPosToWindowPos(double x, double y)
	{
		double O_x, O_y;

		if (m_UserArea.use_percentage)
		{
			O_x = m_Width * m_UserArea.left;
			O_y = m_Height * m_UserArea.top;
		}
		else
		{
			O_x = m_UserArea.left;
			O_y = m_UserArea.top;
		}

		return { x + O_x, y + O_y };
	}
}