export module Input;

export import :KeyInput;
export import :MouseInput;

namespace ACFT
{
	export enum class Controller
	{
		mouse = 0, keyboard
	};

	export bool IsKeyDown(Controller type, int keycode);
}
