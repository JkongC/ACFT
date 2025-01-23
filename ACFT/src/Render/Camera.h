#ifndef ACFT_CAMERA_H_
#define ACFT_CAMERA_H_

#include "Event/EventManager.h"
#include "Tick/Tick.h"

namespace ACFT 
{
	class Camera : public EventListener, public LogicTicker
	{
	public:
		static inline Camera& GetInstance()
		{
			static Camera g_Camera;
			return g_Camera;
		}

		void UpdateAllVec();

		glm::mat4 GetVP();

		void HandleEvent(const Event& event) override;

		void TickLogic(float delta);

	public:
		inline const float& GetYaw() const { return yaw; }
		inline void SetYaw(const float& new_yaw) { yaw = new_yaw; }

		inline const float& GetPitch() const { return pitch; }
		inline void SetPitch(const float& new_pitch) { pitch = new_pitch; }

		inline const glm::vec3& GetPos() const { return pos; }
		inline void SetPos(const glm::vec3& new_pos) { pos = new_pos; }

		inline glm::vec3 GetLookingVec() { return looking; }
		inline glm::vec3 GetUpVec() { return up; }
		inline glm::vec3 GetRightVec() { return right; }

	private:
		Camera();
		~Camera();
		Camera(const Camera&) = delete;
		Camera(Camera&&) = delete;

		float yaw;
		float pitch;

		float last_yaw;
		float last_pitch;

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
	};
}

#endif
