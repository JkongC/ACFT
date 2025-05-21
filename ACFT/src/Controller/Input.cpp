module;

#include <glew.h>
#include <glfw3.h>

module Input;

import <unordered_map>;
import <array>;

import Config;
import Log;

namespace ACFT
{
	constexpr std::array<int, KeyCount> GLKeyCodes
	{
		GLKeyCode::SPACE,
		GLKeyCode::APOSTROPHE,
		GLKeyCode::COMMA,
		GLKeyCode::MINUS,
		GLKeyCode::PERIOD,
		GLKeyCode::SLASH,
		GLKeyCode::_0,
		GLKeyCode::_1,
		GLKeyCode::_2,
		GLKeyCode::_3,
		GLKeyCode::_4,
		GLKeyCode::_5,
		GLKeyCode::_6,
		GLKeyCode::_7,
		GLKeyCode::_8,
		GLKeyCode::_9,
		GLKeyCode::SEMICOLON,
		GLKeyCode::EQUAL,
		GLKeyCode::A,
		GLKeyCode::B,
		GLKeyCode::C,
		GLKeyCode::D,
		GLKeyCode::E,
		GLKeyCode::F,
		GLKeyCode::G,
		GLKeyCode::H,
		GLKeyCode::I,
		GLKeyCode::J,
		GLKeyCode::K,
		GLKeyCode::L,
		GLKeyCode::M,
		GLKeyCode::N,
		GLKeyCode::O,
		GLKeyCode::P,
		GLKeyCode::Q,
		GLKeyCode::R,
		GLKeyCode::S,
		GLKeyCode::T,
		GLKeyCode::U,
		GLKeyCode::V,
		GLKeyCode::W,
		GLKeyCode::X,
		GLKeyCode::Y,
		GLKeyCode::Z,
		GLKeyCode::LEFT_BRACKET,
		GLKeyCode::BACKSLASH,
		GLKeyCode::RIGHT_BRACKET,
		GLKeyCode::GRAVE_ACCENT,
		GLKeyCode::WORLD_1,
		GLKeyCode::WORLD_2,
		GLKeyCode::ESCAPE,
		GLKeyCode::ENTER,
		GLKeyCode::TAB,
		GLKeyCode::BACKSPACE,
		GLKeyCode::INSERT,
		GLKeyCode::DELETE,
		GLKeyCode::RIGHT,
		GLKeyCode::LEFT,
		GLKeyCode::DOWN,
		GLKeyCode::UP,
		GLKeyCode::PAGE_UP,
		GLKeyCode::PAGE_DOWN,
		GLKeyCode::HOME,
		GLKeyCode::END,
		GLKeyCode::CAPS_LOCK,
		GLKeyCode::SCROLL_LOCK,
		GLKeyCode::NUM_LOCK,
		GLKeyCode::PRINT_SCREEN,
		GLKeyCode::PAUSE,
		GLKeyCode::F1,
		GLKeyCode::F2,
		GLKeyCode::F3,
		GLKeyCode::F4,
		GLKeyCode::F5,
		GLKeyCode::F6,
		GLKeyCode::F7,
		GLKeyCode::F8,
		GLKeyCode::F9,
		GLKeyCode::F10,
		GLKeyCode::F11,
		GLKeyCode::F12,
		GLKeyCode::F13,
		GLKeyCode::F14,
		GLKeyCode::F15,
		GLKeyCode::F16,
		GLKeyCode::F17,
		GLKeyCode::F18,
		GLKeyCode::F19,
		GLKeyCode::F20,
		GLKeyCode::F21,
		GLKeyCode::F22,
		GLKeyCode::F23,
		GLKeyCode::F24,
		GLKeyCode::F25,
		GLKeyCode::KP_0,
		GLKeyCode::KP_1,
		GLKeyCode::KP_2,
		GLKeyCode::KP_3,
		GLKeyCode::KP_4,
		GLKeyCode::KP_5,
		GLKeyCode::KP_6,
		GLKeyCode::KP_7,
		GLKeyCode::KP_8,
		GLKeyCode::KP_9,
		GLKeyCode::KP_DECIMAL,
		GLKeyCode::KP_DIVIDE,
		GLKeyCode::KP_MULTIPLY,
		GLKeyCode::KP_SUBTRACT,
		GLKeyCode::KP_ADD,
		GLKeyCode::KP_ENTER,
		GLKeyCode::KP_EQUAL,
		GLKeyCode::LEFT_SHIFT,
		GLKeyCode::LEFT_CONTROL,
		GLKeyCode::LEFT_ALT,
		GLKeyCode::LEFT_SUPER,
		GLKeyCode::RIGHT_SHIFT,
		GLKeyCode::RIGHT_CONTROL,
		GLKeyCode::RIGHT_ALT,
		GLKeyCode::RIGHT_SUPER,
		GLKeyCode::MENU,
		GLKeyCode::MOUSE_LEFT,
		GLKeyCode::MOUSE_RIGHT,
		GLKeyCode::MOUSE_MIDDLE
	};

	Key::operator int() const
	{
		switch (Config::GetRenderAPI())
		{
		case RenderAPI::OpenGL:
			if (m_ID >= GLKeyCodes.size())
			{
				ACFT_LOG_WARN("Using an invalid key id \"{}\" !", m_ID);
				return GLKeyCodes[0];
			}
			else
			{
				return GLKeyCodes[m_ID];
			}
			break;
		default:
			ACFT_LOG_WARN("Using an invalid render API!");
			return GLKeyCodes[0];
			break;
		}
	}

	bool IsKeyDown(const Ref<Window>& window, const Key& key)
	{
		switch (Config::GetRenderAPI())
		{
		case RenderAPI::OpenGL:
			return glfwGetKey(static_cast<GLFWwindow*>(window->GetRawWindow()), key);
		default:
			ACFT_LOG_WARN("Using an invalid render API!");
			return false;
			break;
		}
	}

	std::pair<double, double> GetCursorPos(const Ref<Window>& window)
	{
		std::pair<double, double> ret;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window->GetRawWindow()), &ret.first, &ret.second);
		return ret;
	}

}
