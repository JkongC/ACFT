export module Input;

export import :KeyInput;
export import :MouseInput;

namespace ACFT
{
	export enum class Controller
	{
		mouse = 0, keyboard
	};

	export class Key
	{
	private:

	};

	export bool operator==(const int& keycode, const Key& key_obj);

	export bool IsKeyDown(const Key& key);
}
