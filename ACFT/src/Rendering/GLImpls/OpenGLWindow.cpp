module;

#include <stbi/stb_image.h>

#include <glew.h>
#include <glfw3.h>

#if defined (ACFT_PLATFORM_WINDOWS)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>
#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")
#endif

#include <entt/entity/registry.hpp>

#include <atomic>

module Window:OpenGLWindow;

import Log;
import Event;
import Config;

#include "gldbg.h"

namespace ACFT
{
	std::atomic<size_t> WindowCount{ 0 };
	
	GLFWimage icon_img[1];
	
	OpenGLWindow::OpenGLWindow(bool caption_bar)
		: m_Info(MakeScope<WindowInfo>())
	{
		m_HasCaptionBar = caption_bar;

		if (!glfwInit())
		{
			ACFT_LOG_FATAL("Failed to initialize glfw!");
			exit(-1);
		}

		glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
		glfwWindowHint(GLFW_AUTO_ICONIFY, GL_FALSE);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, GL_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GLFW_DECORATED, static_cast<int>(caption_bar));

		m_RawWindow = glfwCreateWindow(m_Width, m_Height, Config::GetWindowName().data(), nullptr, nullptr);
		if (!m_RawWindow)
		{
			glfwTerminate();
			ACFT_LOG_FATAL("Failed to initialize the window!");
			exit(-1);
		}

		if (auto& icon = Config::GetWindowIcon();
			icon.GetInternalData() != nullptr)
		{
			icon_img[0].pixels = icon.GetInternalData();
			icon_img[0].width = icon.GetWidth();
			icon_img[0].height = icon.GetHeight();
			glfwSetWindowIcon(m_RawWindow, 1, icon_img);
		}

#if defined (ACFT_PLATFORM_WINDOWS)
		if (!caption_bar)
		{
			HWND handle = glfwGetWin32Window(m_RawWindow);

			DWM_WINDOW_CORNER_PREFERENCE corner = DWMWCP_ROUND;
			DwmSetWindowAttribute(handle, DWMWA_WINDOW_CORNER_PREFERENCE, &corner, sizeof(corner));

			SetWindowPos(handle, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
		}
#endif

		glfwMakeContextCurrent(m_RawWindow);

		if (glewInit() != GLEW_OK)
		{
			ACFT_LOG_FATAL("Failed to initialize glew!");
			exit(-1);
		}

		ACFT_GL_LOG("OpenGL Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		
		glfwSetCursorPosCallback(m_RawWindow, caption_bar ? MousePosCallback : MousePosCallbackBorderless);
		glfwSetMouseButtonCallback(m_RawWindow, caption_bar ? MouseButtonCallback : MouseButtonCallbackBorderless);
		glfwSetKeyCallback(m_RawWindow, KeyCallback);
		glfwSetFramebufferSizeCallback(m_RawWindow, WindowResizeCallback);

		glfwSetWindowUserPointer(m_RawWindow, m_Info.get());

		glfwGetWindowContentScale(m_RawWindow, &m_Info->ScaleX, &m_Info->ScaleY);

		WindowCount++;
	}

	OpenGLWindow::~OpenGLWindow()
	{
		glfwDestroyWindow(m_RawWindow);
		WindowCount--;
		if (WindowCount == 0)
			glfwTerminate();
	}

	std::pair<int, int> OpenGLWindow::GetPos()
	{
		std::pair<int, int> ret{};
		glfwGetWindowPos(m_RawWindow, &ret.first, &ret.second);
		return ret;
	}

	void OpenGLWindow::SetPos(int x, int y)
	{
		glfwSetWindowPos(m_RawWindow, x, y);
	}

	bool OpenGLWindow::ShouldClose()
	{
		return glfwWindowShouldClose(m_RawWindow);
	}

	void OpenGLWindow::PollEvents()
	{
		glfwPollEvents();
	}

	void OpenGLWindow::WaitEvents()
	{
		glfwWaitEvents();
	}

	void OpenGLWindow::Minimize()
	{
		glfwIconifyWindow(m_RawWindow);
	}

	void OpenGLWindow::Maximize()
	{
		glfwMaximizeWindow(m_RawWindow);
	}

	void OpenGLWindow::MousePosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		EventManager::Global().DistributeEvent(Events::MOUSE_POS,
			MousePosInfo{ xpos, ypos }
		);
	}
	
	void OpenGLWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		EventManager::Global().DistributeEvent(Events::MOUSE_BUTTON,
			MouseButtonInfo{ button, action == GLFW_PRESS, mods }
		);
	}

	void OpenGLWindow::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		EventManager::Global().DistributeEvent(Events::SCROLL,
			ScrollInfo{ xoffset, yoffset }
		);
	}

	void OpenGLWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		EventManager::Global().DistributeEvent(Events::KEY,
			KeyInfo{ key, action == GLFW_PRESS, scancode, mods }
		);
	}

	void OpenGLWindow::WindowResizeCallback(GLFWwindow* window, int width, int height)
	{
		EventManager::Global().DistributeEvent(Events::WINDOW_RESIZE,
			WindowSizeInfo{ width, height }
		);
	}

	void OpenGLWindow::MousePosCallbackBorderless(GLFWwindow* window, double xpos, double ypos)
	{	
#if defined (ACFT_PLATFORM_WINDOWS)
		WindowInfo& info = *static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));
		
		if (info.Dragging)
		{
			HWND hwnd = glfwGetWin32Window(window);
			POINT cursor_pos;
			GetCursorPos(&cursor_pos);
			int new_window_x = info.WindowStartX + (cursor_pos.x - info.DragStartX);
			int new_window_y = info.WindowStartY + (cursor_pos.y - info.DragStartY);
			SetWindowPos(hwnd, NULL, new_window_x, new_window_y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE | SWP_NOCOPYBITS);
		}
#endif
		EventManager::Global().DistributeEvent(Events::MOUSE_POS,
			MousePosInfo{ xpos, ypos }
		);
	}

	void OpenGLWindow::MouseButtonCallbackBorderless(GLFWwindow* window, int button, int action, int mods)
	{
		EventManager::Global().DistributeEvent(Events::MOUSE_BUTTON,
			MouseButtonInfo{ button, action == GLFW_PRESS, mods }
		);
#if defined (ACFT_PLATFORM_WINDOWS)
		WindowInfo& info = *static_cast<WindowInfo*>(glfwGetWindowUserPointer(window));

		if (info.CanDrag && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			HWND hwnd = glfwGetWin32Window(window);
			POINT cursor_pos;
			GetCursorPos(&cursor_pos);
			RECT window_rect;
			GetWindowRect(hwnd, &window_rect);
			info.DragStartX = cursor_pos.x;
			info.DragStartY = cursor_pos.y;
			info.WindowStartX = window_rect.left;
			info.WindowStartY = window_rect.top;
			info.Dragging = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action != GLFW_PRESS)
		{
			info.Dragging = false;
		}
#endif
	}

	void OpenGLWindow::SwapFrameBuffers()
	{
		glfwSwapBuffers(m_RawWindow);
	}

	void* OpenGLWindow::GetRawWindow()
	{
		return m_RawWindow;
	}

	void OpenGLWindow::MakeContextCurrent()
	{
		glfwMakeContextCurrent(m_RawWindow);
	}

	void OpenGLWindow::DetachContext()
	{
		glfwMakeContextCurrent(nullptr);
	}

	void OpenGLWindow::SetCursor(CursorType cursor)
	{
		GLFWcursor* new_cursor = nullptr;

		switch (cursor)
		{
		case CursorType::input:
			new_cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
			break;
		case CursorType::crosshair:
			new_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
			break;
		case CursorType::pointing_hand:
			new_cursor = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);
			break;
		case CursorType::h_resize:
			new_cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
			break;
		case CursorType::v_resize:
			new_cursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
			break;
		case CursorType::nwse_resize:
			new_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
			break;
		case CursorType::nesw_resize:
			new_cursor = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
			break;
		case CursorType::all_resize:
			new_cursor = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
			break;
		default:
			break;
		}

		glfwSetCursor(m_RawWindow, new_cursor);
	}

	void OpenGLWindow::SetDragAbility(bool allow_drag)
	{
		m_Info->CanDrag = allow_drag;
	}

	void OpenGLWindow::MarkShouldClose()
	{
		glfwSetWindowShouldClose(m_RawWindow, GLFW_TRUE);
	}
}