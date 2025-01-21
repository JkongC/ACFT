#include "stdafx.h"

#include "ACFT.h"
#include "Util/Vec.h"
#include "Camera.h"

namespace ACFT
{
	Camera::Camera()
		: yaw(PI / 2), pitch(0.0), pos(0.0f, 0.0f, 2.0f),
		looking(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), right(1.0f, 0.0f, 0.0f),
		mouse_xpos((double)WindowWidth / 2.0), mouse_ypos((double)WindowHeight / 2.0)
	{
		EventSubscribe(Event::Type::mouse_move);
		EventSubscribe(Event::Type::mouse_keyup);
		EventSubscribe(Event::Type::key_release);
	}
	
	void Camera::UpdateAllVec()
	{
		if (this->yaw > 2 * PI) this->yaw -= 2 * PI;
		if (this->yaw < 0.0f) this->yaw += 2 * PI;
		this->pitch = std::clamp(this->pitch, -PI / 2, PI / 2);
		
		this->looking = GetVec3fFromYP(this->yaw, this->pitch);
		if (pitch > 0)
		{
			this->up = GetVec3fFromYP(-this->yaw, PI / 2 - this->pitch);
		}
		else {
			this->up = GetVec3fFromYP(this->yaw, this->pitch + PI / 2);
		}
		this->right = glm::normalize(glm::cross(this->looking, this->up));
	}

	glm::mat4 Camera::GetVP()
	{
		UpdateAllVec();

		glm::mat4 view = glm::lookAt(this->pos, this->pos + this->looking, this->up);
		glm::mat4 perspective = glm::perspective(PI / 4, (float)WindowWidth / (float)WindowHeight, 0.1f, 100.0f);
		return perspective * view;
	}

	void Camera::HandleEvent(const Event& event)
	{
		switch (event.GetType())
		{
		case Event::Type::mouse_move:
		{
			if (!lock_mouse)
				break;
			const InputEvent& input = static_cast<const InputEvent&>(event);

			this->yaw -= (input.xpos - this->mouse_xpos) / 800.0;
			this->mouse_xpos = input.xpos;

			this->pitch -= (input.ypos - this->mouse_ypos) / 800.0;
			this->mouse_ypos = input.ypos;
		}
			break;

		case Event::Type::mouse_keyup:
		{
			if (lock_mouse)
				break;
			const InputEvent& input = static_cast<const InputEvent&>(event);

			if (input.keycode == GLFW_MOUSE_BUTTON_LEFT)
			{
				lock_mouse = true;
				glfwSetInputMode(Game::GetGameWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				glfwSetCursorPos(Game::GetGameWindow(), WindowWidth / 2.0, WindowHeight / 2.0);
				mouse_xpos = WindowWidth / 2.0;
				mouse_ypos = WindowHeight / 2.0;
			}
		}
			break;

		case Event::Type::key_release:
		{
			if (!lock_mouse)
				break;
			const InputEvent& input = static_cast<const InputEvent&>(event);

			if (input.keycode == GLFW_KEY_ESCAPE)
			{
				lock_mouse = false;
				glfwSetInputMode(Game::GetGameWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{
				switch (input.keycode)
				{
				case GLFW_KEY_W:
					W_pressed = true;
					break;
				case GLFW_KEY_A:
					A_pressed = true;
					break;
				case GLFW_KEY_S:
					S_pressed = true;
					break;
				case GLFW_KEY_D:
					D_pressed = true;
					break;
				default:
					break;
				}
			}
		}
			break;

		default:
			break;
		}
	}
}