module;

#include <glew.h>
#include <glfw3.h>

export module Window:OpenGLWindow;

import <variant>;

import Window;
import ACFT.EnhancingFuncs;

namespace ACFT
{
	export class OpenGLWindow : public Window
	{
	public:
		OpenGLWindow(int width, int height, bool caption_bar);
		~OpenGLWindow();

		virtual std::pair<int, int> GetPos() override;
		virtual void SetPos(int x, int y) override;

		virtual bool ShouldClose() override;
		
		virtual void PollEvents() override;

		virtual void WaitEvents() override;

		virtual void SwapFrameBuffers() override;

		virtual void* GetRawWindow() override;

		virtual void MakeContextCurrent() override;
		
		virtual void DetachContext() override;

		virtual void SetCursor(CursorType cursor) override;
		virtual void SetDragAbility(bool allow_drag) override;

		virtual void Minimize() override;
		virtual void Maximize() override;

		virtual void MarkShouldClose() override;

		virtual void SetOpacity(float alpha) override;

	private:
		static void MousePosCallback(GLFWwindow* window, double xpos, double ypos);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void WindowResizeCallback(GLFWwindow* window, int width, int height);

		static void MousePosCallbackBorderless(GLFWwindow* window, double xpos, double ypos);
		static void MouseButtonCallbackBorderless(GLFWwindow* window, int button, int action, int mods);

	private:
		GLFWwindow* m_RawWindow{nullptr};

		struct WindowInfo;

		WindowInfo* m_Info;
	};
}