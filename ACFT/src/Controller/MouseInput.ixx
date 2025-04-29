export module Input:MouseInput;

export import <utility>;

import Types;
import Window;

namespace ACFT
{
	export namespace GLKeyCode
	{
		constexpr int MOUSE_LEFT = 0;
		constexpr int MOUSE_RIGHT = 1;
		constexpr int MOUSE_MIDDLE = 2;
	}

	export std::pair<double, double> GetCursorPos(Ref<Window> window);
}
