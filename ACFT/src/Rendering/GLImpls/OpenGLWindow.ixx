module;

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

		virtual bool ShouldClose() override;
		
		virtual void PollEvents() override;

		virtual void SwapFrameBuffers() override;

		virtual void* GetRawWindow() override;

	private:
		static void MousePosCallback(GLFWwindow* window, double xpos, double ypos);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void WindowResizeCallback(GLFWwindow* window, int width, int height);

	private:
		GLFWwindow* m_RawWindow{nullptr};
	};
}