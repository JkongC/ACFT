#include "stdafx.h"

#include "ACFT.h"
#include "InputQuery.h"

namespace ACFT::Input
{
	bool IsKeyDown(int keycode)
	{
		int result = glfwGetKey(Game::GetGameWindow(), keycode);
		return result == GLFW_PRESS || result == GLFW_REPEAT;
	}
}