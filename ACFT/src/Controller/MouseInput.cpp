module;

#include <glew.h>
#include <glfw3.h>

module Input:MouseInput;

namespace ACFT
{
	std::pair<double, double> GetCursorPos(Ref<Window> window)
	{
		std::pair<double, double> ret;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window->GetRawWindow()), &ret.first, &ret.second);
		return ret;
	}
}
