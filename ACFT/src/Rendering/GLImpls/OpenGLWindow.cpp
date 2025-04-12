module;

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>

#include <glew.h>
#include <glfw3.h>

#include <atomic>

module Window:OpenGLWindow;

import Log;
import Event;

namespace ACFT
{
	std::atomic<size_t> WindowCount{ 0 };
	
	GLFWimage icon[1];
	
	OpenGLWindow::OpenGLWindow()
	{
		if (!glfwInit())
		{
			ACFT_LOG_FATAL("Failed to initialize glfw!");
			exit(-1);
		}

		icon[0].pixels = stbi_load("resources/acft_icon.png", &icon[0].width, &icon[0].height, nullptr, 4);

		m_RawWindow = glfwCreateWindow(m_Width, m_Height, "AnotherCraft", nullptr, nullptr);
		if (!m_RawWindow)
		{
			glfwTerminate();
			ACFT_LOG_FATAL("Failed to initialize the window!");
			exit(-1);
		}

		glfwSetWindowIcon(m_RawWindow, 1, icon);

		glfwMakeContextCurrent(m_RawWindow);

		if (glewInit() != GLEW_OK)
		{
			ACFT_LOG_FATAL("Failed to initialize glew!");
			exit(-1);
		}

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

	void OpenGLWindow::MousePosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		EventManager::DistributeEvent(Events::MOUSE_POS,
			MousePosInfo{ xpos, ypos }
		);
	}
	
	void OpenGLWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		EventManager::DistributeEvent(Events::MOUSE_BUTTON,
			MouseButtonInfo{ button, action == GLFW_PRESS, mods }
		);
	}

	void OpenGLWindow::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		EventManager::DistributeEvent(Events::SCROLL,
			ScrollInfo{ xoffset, yoffset }
		);
	}

	void OpenGLWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		EventManager::DistributeEvent(Events::KEY,
			KeyInfo{ key, action == GLFW_PRESS, scancode, mods }
		);
	}

	void OpenGLWindow::WindowResizeCallback(GLFWwindow* window, int width, int height)
	{
		EventManager::DistributeEvent(Events::WINDOW_RESIZE,
			WindowSizeInfo{ width, height }
		);
	}
}