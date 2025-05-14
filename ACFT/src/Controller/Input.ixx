export module Input;

import :KeyInput;
import :MouseInput;

namespace ACFT
{
	export enum class Controller
	{
		mouse = 0, keyboard
	};

	export class Key
	{
	public:
		constexpr Key(unsigned int id) : m_ID(id) {}

		operator int() const;

	private:
		unsigned int m_ID;
	};

	export constexpr int KeyCount = 123;
	
	export namespace Keys
	{
		// Keyboard
		constexpr Key SPACE						= 0;
		constexpr Key APOSTROPHE				= 1; /* ' */
		constexpr Key COMMA						= 2; /* , */
		constexpr Key MINUS						= 3; /* - */
		constexpr Key PERIOD					= 4; /* . */
		constexpr Key SLASH						= 5; /* / */
		constexpr Key _0						= 6;
		constexpr Key _1						= 7;
		constexpr Key _2						= 8;
		constexpr Key _3						= 9;
		constexpr Key _4						= 10;
		constexpr Key _5						= 11;
		constexpr Key _6						= 12;
		constexpr Key _7						= 13;
		constexpr Key _8						= 14;
		constexpr Key _9						= 15;
		constexpr Key SEMICOLON					= 16;/* ; */
		constexpr Key EQUAL						= 17;  /* = */
		constexpr Key A							= 18;
		constexpr Key B							= 19;
		constexpr Key C							= 20;
		constexpr Key D							= 21;
		constexpr Key E							= 22;
		constexpr Key F							= 23;
		constexpr Key G							= 24;
		constexpr Key H							= 25;
		constexpr Key I							= 26;
		constexpr Key J							= 27;
		constexpr Key K							= 28;
		constexpr Key L							= 29;
		constexpr Key M							= 30;
		constexpr Key N							= 31;
		constexpr Key O							= 32;
		constexpr Key P							= 33;
		constexpr Key Q							= 34;
		constexpr Key R							= 35;
		constexpr Key S							= 36;
		constexpr Key T							= 37;
		constexpr Key U							= 38;
		constexpr Key V							= 39;
		constexpr Key W							= 40;
		constexpr Key X							= 41;
		constexpr Key Y							= 42;
		constexpr Key Z							= 43;
		constexpr Key LEFT_BRACKET				= 44; /* [ */
		constexpr Key BACKSLASH					= 45; /* \ */
		constexpr Key RIGHT_BRACKET				= 46; /* ] */
		constexpr Key GRAVE_ACCENT				= 47; /* ` */
		constexpr Key WORLD_1					= 48; /* non-US #1 */
		constexpr Key WORLD_2					= 49;/* non-US #2 */
		constexpr Key ESCAPE					= 50;
		constexpr Key ENTER						= 51;
		constexpr Key TAB						= 52;
		constexpr Key BACKSPACE					= 53;
		constexpr Key INSERT					= 54;
		constexpr Key DELETE					= 55;
		constexpr Key RIGHT						= 56;
		constexpr Key LEFT						= 57;
		constexpr Key DOWN						= 58;
		constexpr Key UP						= 59;
		constexpr Key PAGE_UP					= 60;
		constexpr Key PAGE_DOWN					= 61;
		constexpr Key HOME						= 62;
		constexpr Key END						= 63;
		constexpr Key CAPS_LOCK					= 64;
		constexpr Key SCROLL_LOCK				= 65;
		constexpr Key NUM_LOCK					= 66;
		constexpr Key PRINT_SCREEN				= 67;
		constexpr Key PAUSE						= 68;
		constexpr Key F1						= 69;
		constexpr Key F2						= 70;
		constexpr Key F3						= 71;
		constexpr Key F4						= 72;
		constexpr Key F5						= 73;
		constexpr Key F6						= 74;
		constexpr Key F7						= 75;
		constexpr Key F8						= 76;
		constexpr Key F9						= 77;
		constexpr Key F10						= 78;
		constexpr Key F11						= 79;
		constexpr Key F12						= 80;
		constexpr Key F13						= 81;
		constexpr Key F14						= 82;
		constexpr Key F15						= 83;
		constexpr Key F16						= 84;
		constexpr Key F17						= 85;
		constexpr Key F18						= 86;
		constexpr Key F19						= 87;
		constexpr Key F20						= 88;
		constexpr Key F21						= 89;
		constexpr Key F22						= 90;
		constexpr Key F23						= 91;
		constexpr Key F24						= 92;
		constexpr Key F25						= 93;
		constexpr Key KP_0						= 94;
		constexpr Key KP_1						= 95;
		constexpr Key KP_2						= 96;
		constexpr Key KP_3						= 97;
		constexpr Key KP_4						= 98;
		constexpr Key KP_5						= 99;
		constexpr Key KP_6						= 100;
		constexpr Key KP_7						= 101;
		constexpr Key KP_8						= 102;
		constexpr Key KP_9						= 103;
		constexpr Key KP_DECIMAL				= 104;
		constexpr Key KP_DIVIDE					= 105;
		constexpr Key KP_MULTIPLY				= 106;
		constexpr Key KP_SUBTRACT				= 107;
		constexpr Key KP_ADD					= 108;
		constexpr Key KP_ENTER					= 109;
		constexpr Key KP_EQUAL					= 110;
		constexpr Key LEFT_SHIFT				= 111;
		constexpr Key LEFT_CONTROL				= 112;
		constexpr Key LEFT_ALT					= 113;
		constexpr Key LEFT_SUPER				= 114;
		constexpr Key RIGHT_SHIFT				= 115;
		constexpr Key RIGHT_CONTROL				= 116;
		constexpr Key RIGHT_ALT					= 117;
		constexpr Key RIGHT_SUPER				= 118;
		constexpr Key MENU						= 119;
		constexpr Key LAST = MENU;

		constexpr Key MOUSE_LEFT = 120;
		constexpr Key MOUSE_RIGHT = 121;
		constexpr Key MOUSE_MIDDLE = 122;
	}

	export bool IsKeyDown(const Key& key);
}
