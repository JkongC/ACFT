module;

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <glew.h>
#include <glfw3.h>

export module Window:OpenGLWindow;

import Window;

namespace ACFT
{
	export class OpenGLWindow : public Window
	{
	public:
		OpenGLWindow();
		~OpenGLWindow();

	private:
		GLFWwindow* m_RawWindow{nullptr};
	};
}