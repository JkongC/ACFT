#include "stdafx.h"

#include "ACFT.h"
#include "Util/Vec.h"
#include "Camera.h"

namespace ACFT
{
	Camera::Camera()
		: yaw(PI / 2), pitch(0.0), last_yaw(yaw), last_pitch(pitch), pos(0.0f, 0.0f, 2.0f)
		, looking(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), right(1.0f, 0.0f, 0.0f)
		, mouse_xpos((double)WindowWidth / 2.0), mouse_ypos((double)WindowHeight / 2.0)
	{
		EventSubscribe(Event::Type::mouse_move);
		EventSubscribe(Event::Type::mouse_keyup);
		EventSubscribe(Event::Type::key_release);

		TickManager::GetInstance().AddLogicTicker(this);
	}

	Camera::~Camera()
	{
		EventUnsubscribe(Event::Type::mouse_move);
		EventUnsubscribe(Event::Type::mouse_keyup);
		EventUnsubscribe(Event::Type::key_release);
	}
	
	void Camera::UpdateAllVec()
	{
		float delta_time = TickManager::GetInstance().GetTimeDelta();
		
		while (this->yaw > 2 * PI)
		{
			this->yaw -= 2 * PI;
			this->last_yaw -= 2 * PI;
		}

		while (this->yaw < 0.0f)
		{
			this->yaw += 2 * PI;
			this->last_yaw += 2 * PI;
		}

		this->pitch = std::clamp(this->pitch, -PI / 2, PI / 2);

		float lerp_yaw = last_yaw + (yaw - last_yaw) * (delta_time/ LogicMSPT);
		float lerp_pitch = last_pitch + (pitch - last_pitch) * (delta_time / LogicMSPT);
		
		this->looking = GetVec3fFromYP(lerp_yaw, lerp_pitch);
		if (pitch > 0)
		{
			this->up = GetVec3fFromYP(-lerp_yaw, PI / 2 - lerp_pitch);
		}
		else {
			this->up = GetVec3fFromYP(lerp_yaw, lerp_pitch + PI / 2);
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

	void Camera::TickLogic(float delta)
	{
		last_yaw = yaw;
		last_pitch = pitch;
	}
}