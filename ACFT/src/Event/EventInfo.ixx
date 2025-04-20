export module EventInfo;

namespace ACFT
{
	export struct MousePosInfo
	{
		double x;
		double y;

		bool operator!=(const MousePosInfo&) const = default;
	};

	export struct MouseButtonInfo
	{
		int keycode;
		bool pressed;
		int mods;

		bool operator!=(const MouseButtonInfo&) const = default;
	};

	export struct KeyInfo
	{
		int keycode;
		bool pressed;
		int scancode;
		int mods;

		bool operator!=(const KeyInfo&) const = default;
	};

	export struct WindowSizeInfo
	{
		int width;
		int height;

		bool operator!=(const WindowSizeInfo&) const = default;
	};

	export struct ScrollInfo
	{
		double xoffset;
		double yoffset;

		bool operator!=(const ScrollInfo&) const = default;
	};
}
