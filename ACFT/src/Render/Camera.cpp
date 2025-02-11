#include "stdafx.h"

#include "ACFT.h"
#include "Util/Vec.h"
#include "Camera.h"
#include "Controller/InputQuery.h"
#include "Util/MathHelper.h"

namespace ACFT
{
	namespace
	{
		constexpr float sqrts[3] = { Sqrt<float>(1.0), Sqrt<float>(2.0), Sqrt<float>(3.0) };
	}
	
	Camera::Camera()
		: yaw(PI / 2), pitch(0.0), prev_yaw(yaw), prev_pitch(pitch), pos(0.0f, 0.0f, 2.0f)
		, looking(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), right(1.0f, 0.0f, 0.0f)
		, mouse_xpos((double)WindowWidth / 2.0), mouse_ypos((double)WindowHeight / 2.0)
		, speed(0.0f, 0.0f ,0.0f), axis_speed(0.0f, 0.0f, 0.0f)
	{
		EventSubscribe(Event::Type::mouse_move);
		EventSubscribe(Event::Type::mouse_keyup);
		EventSubscribe(Event::Type::key_release);
		EventSubscribe(Event::Type::key_press);
	}

	Camera::~Camera()
	{
		EventUnsubscribe(Event::Type::mouse_move);
		EventUnsubscribe(Event::Type::mouse_keyup);
		EventUnsubscribe(Event::Type::key_release);
		EventUnsubscribe(Event::Type::key_press);
	}
	
	void Camera::UpdateAllVec()
	{
		float delta_time = TickManager::GetInstance().GetTimeDelta();

		float pitch_ins = GetPitch();
		pitch_ins = std::clamp(pitch_ins, -PI / 2, PI / 2);

		float yaw_prev = GetPreviousYaw();
		float pitch_prev = GetPreviousPitch();
		
		float lerp_yaw = yaw_prev + (yaw - yaw_prev) * (delta_time / LogicMSPT);
		float lerp_pitch = pitch_prev + (pitch_ins - pitch_prev) * (delta_time / LogicMSPT);

		lerp_pitch = std::clamp(lerp_pitch, -PI / 2, PI / 2);
		
		this->looking = GetVec3fFromYP(lerp_yaw, lerp_pitch);
		if (lerp_pitch > 0)
		{
			this->up = GetVec3fFromYP(lerp_yaw + PI, PI / 2 - lerp_pitch);
		}
		else {
			this->up = GetVec3fFromYP(lerp_yaw, lerp_pitch + PI / 2);
		}
		this->right = glm::normalize(glm::cross(this->looking, this->up));
	}

	void Camera::UpdateAllKeyFlags()
	{
		W_pressed = Input::IsKeyDown(ACFT_KEY_W);
		A_pressed = Input::IsKeyDown(ACFT_KEY_A);
		S_pressed = Input::IsKeyDown(ACFT_KEY_S);
		D_pressed = Input::IsKeyDown(ACFT_KEY_D);
		Shift_pressed = Input::IsKeyDown(ACFT_KEY_LEFT_SHIFT);
		Space_pressed = Input::IsKeyDown(ACFT_KEY_SPACE);
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(this->pos, this->pos + this->looking, this->up);
	}

	glm::mat4 Camera::GetProjectionMatrix()
	{
		static glm::mat4 proj = glm::perspective(glm::radians(45.0f), static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight),
			0.1f, 1000.0f);
		return proj;
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

			float yaw_prev = GetYaw();
			float pitch_prev = GetPitch();

			float yaw_after = yaw_prev - static_cast<float>((input.xpos - this->mouse_xpos) / 800.0);
			float pitch_after = pitch_prev - static_cast<float>((input.ypos - this->mouse_ypos) / 800.0);

			while (yaw_after > 2 * PI)
			{
				yaw_after -= 2 * PI;
				this->prev_yaw -= 2 * PI;
			}

			while (yaw_after < 0.0f)
			{
				yaw_after += 2 * PI;
				this->prev_yaw += 2 * PI;
			}

			SetYaw(yaw_after);
			this->mouse_xpos = input.xpos;

			if (pitch_after < PI / 2 && pitch_after > -PI / 2)
			{
				SetPitch(pitch_after);
			}
			this->mouse_ypos = input.ypos;

			should_update_mat = true;
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
			const InputEvent& input = static_cast<const InputEvent&>(event);

			if (input.keycode == ACFT_KEY_ESCAPE)
			{
				if (!lock_mouse)
					break;
				lock_mouse = false;
				glfwSetInputMode(Game::GetGameWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
			break;

		default:
			break;
		}
	}

	void Camera::TickLogic(float delta)
	{
		prev_yaw = GetYaw();
		prev_pitch = GetPitch();
		static float cached_yaw = prev_yaw;

		UpdateAllKeyFlags();
		UpdateAllVec();

		bool x_accelerating = false;
		bool y_accelerating = false;
		bool z_accelerating = false;
		glm::vec3 delta_axis(0.0f, 0.0f, 0.0f);

		glm::vec2 horizontal(axis_speed.x, axis_speed.z);
		if (W_pressed ^ S_pressed && glm::length(horizontal) < max_horizontal_speed)
		{
			if (W_pressed)
			{
				delta_axis.z -= (axis_speed.z > 0.0f ? brake_acceleration : launch_acceleration) * delta;
			}
			else
			{
				delta_axis.z += (axis_speed.z < 0.0f ? brake_acceleration : launch_acceleration) * delta;
			}
			z_accelerating = true;
		}

		if (A_pressed ^ D_pressed && glm::length(horizontal) < max_horizontal_speed)
		{
			if (A_pressed)
			{
				delta_axis.x -= (axis_speed.x > 0.0f ? brake_acceleration : launch_acceleration) * delta;
			}
			else
			{
				delta_axis.x += (axis_speed.x < 0.0f ? brake_acceleration : launch_acceleration) * delta;
			}
			x_accelerating = true;
		}

		if (Space_pressed ^ Shift_pressed && glm::abs(axis_speed.y) < max_vertical_speed)
		{
			if (Space_pressed)
			{
				delta_axis.y += (axis_speed.y < 0 ? brake_acceleration : launch_acceleration) * delta;
			}
			else
			{
				delta_axis.y -= (axis_speed.y > 0 ? brake_acceleration : launch_acceleration) * delta;
			}
			y_accelerating = true;
		}

		int accelerating_axis_count = x_accelerating + y_accelerating + z_accelerating;
		if (accelerating_axis_count)
		{
			delta_axis /= static_cast<float>(sqrts[accelerating_axis_count - 1]);
			axis_speed += delta_axis;

			if (!x_accelerating && glm::abs(axis_speed.x) > friction_acceleration * delta)
			{
				axis_speed.x -= (axis_speed.x > 0 ? friction_acceleration : -friction_acceleration) * delta;
			}

			if (!z_accelerating && glm::abs(axis_speed.z) > friction_acceleration * delta)
			{
				axis_speed.z -= (axis_speed.z > 0 ? friction_acceleration : -friction_acceleration) * delta;
			}

			if (!y_accelerating && glm::abs(axis_speed.y) > friction_acceleration * delta)
			{
				axis_speed.y -= (axis_speed.y > 0 ? friction_acceleration : -friction_acceleration) * delta;
			}

			cached_yaw = prev_yaw;
		}

		if (!accelerating_axis_count && glm::length(axis_speed) != 0.0f)
		{
			glm::vec3 reverse_i = glm::normalize(-axis_speed);
			if (glm::dot(axis_speed + reverse_i * friction_acceleration * delta, axis_speed) <= 0.0f)
			{
				axis_speed = glm::zero<glm::vec3>();
			}
			else
			{
				axis_speed += reverse_i * friction_acceleration * delta;
			}
		}

		speed = glm::zero<glm::vec3>();
		speed.y = axis_speed.y;
		if (accelerating_axis_count)
		{
			speed += GetVec3fFromYP(yaw + PI, 0.0f, axis_speed.z);
			speed += GetVec3fFromYP(yaw - PI / 2, 0.0f, axis_speed.x);
		}
		else
		{
			speed += GetVec3fFromYP(cached_yaw + PI, 0.0f, axis_speed.z);
			speed += GetVec3fFromYP(cached_yaw - PI / 2, 0.0f, axis_speed.x);
		}
		pos += speed;

		if (glm::length(speed) != 0)
			should_update_mat = true;
	}
}