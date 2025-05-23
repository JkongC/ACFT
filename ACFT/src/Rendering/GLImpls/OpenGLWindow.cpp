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

		
		glfwSetCursorPosCallback(m_RawWindow, MousePosCallback);
		glfwSetMouseButtonCallback(m_RawWindow, MouseButtonCallback);
		glfwSetKeyCallback(m_RawWindow, KeyCallback);
		glfwSetFramebufferSizeCallback(m_RawWindow, WindowResizeCallback);

		WindowCount++;
	}

	OpenGLWindow::~OpenGLWindow()
	{
		glfwDestroyWindow(m_RawWindow);
		WindowCount--;
		if (WindowCount == 0)
			glfwTerminate();
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
}