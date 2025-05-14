module;

#include <glfw3.h>

export module Input:MouseInput;

export import <utility>;

import Types;
import Window;

namespace ACFT
{
	export namespace GLKeyCode
	{
		constexpr int MOUSE_LEFT = GLFW_MOUSE_BUTTON_LEFT;
		constexpr int MOUSE_RIGHT = GLFW_MOUSE_BUTTON_RIGHT;
		constexpr int MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;
	}

	export std::pair<double, double> GetCursorPos(Ref<Window> window);
}
