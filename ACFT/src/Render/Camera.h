#ifndef ACFT_CAMERA_H_
#define ACFT_CAMERA_H_

#include "Event/EventManager.h"
#include "Tick/Tick.h"

namespace ACFT 
{
	class Camera : public EventListener, virtual public LogicTicker
	{
	public:
		static inline Camera& GetInstance()
		{
			static Camera g_Camera;
			return g_Camera;
		}

		glm::mat4 GetVP();

		void HandleEvent(const Event& event) override;

		void TickLogic(float delta);

	public:
		inline const float& GetYaw(std::memory_order order = std::memory_order_seq_cst) const { return yaw.load(order); }
		inline void SetYaw(const float& new_yaw, std::memory_order order = std::memory_order_seq_cst) { yaw.store(new_yaw, order); }

		inline const float& GetPitch(std::memory_order order = std::memory_order_seq_cst) const { return pitch.load(order); }
		inline void SetPitch(const float& new_pitch, std::memory_order order = std::memory_order_seq_cst) { pitch.store(new_pitch, order); }

		inline const float& GetPreviousYaw(std::memory_order order = std::memory_order_seq_cst) const { return prev_yaw.load(order); }
		inline void SetPreviousYaw(const float& new_yaw, std::memory_order order = std::memory_order_seq_cst) { prev_yaw.store(new_yaw, order); }

		inline const float& GetPreviousPitch(std::memory_order order = std::memory_order_seq_cst) const { return prev_pitch.load(order); }
		inline void SetPreviousPitch(const float& new_pitch, std::memory_order order = std::memory_order_seq_cst) { prev_pitch.store(new_pitch, order); }

		inline const glm::vec3& GetPos() const { return pos; }
		inline void SetPos(const glm::vec3& new_pos) { pos = new_pos; }

		inline glm::vec3 GetLookingVec() { return looking; }
		inline glm::vec3 GetUpVec() { return up; }
		inline glm::vec3 GetRightVec() { return right; }

		inline float GetSpeed() { return glm::length(axis_speed); }

	private:
		Camera();
		~Camera();
		Camera(const Camera&) = delete;
		Camera(Camera&&) = delete;

		void UpdateAllVec();

		void UpdateAllKeyFlags();

	private:

		std::atomic<float> yaw;
		std::atomic<float> pitch;

		std::atomic<float> prev_yaw;
		std::atomic<float> prev_pitch;

		glm::vec3 pos;
		glm::vec3 looking;
		glm::vec3 up;
		glm::vec3 right;

		double mouse_xpos;
		double mouse_ypos;

		bool lock_mouse = false;

		bool W_pressed = false;
		bool A_pressed = false;
		bool S_pressed = false;
		bool D_pressed = false;
		bool Space_pressed = false;
		bool Shift_pressed = false;

		glm::vec3 speed;
		glm::vec3 axis_speed;

		//Consts
		const float max_horizontal_speed = 0.03f;
		const float max_vertical_speed = 0.015f;
		const float launch_acceleration = 0.00006f;
		const float friction_acceleration = 0.00008f;
		const float brake_acceleration = 0.0001f;
	};
}

#endif
