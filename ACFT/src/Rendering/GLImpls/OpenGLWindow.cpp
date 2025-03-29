module;

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>

#include <glew.h>
#include <glfw3.h>

module Window:OpenGLWindow;

import Log;

namespace ACFT
{
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
	}

	OpenGLWindow::~OpenGLWindow()
	{
		glfwTerminate();
	}
}