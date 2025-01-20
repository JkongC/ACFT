#ifndef ACFT_CAMERA_H_
#define ACFT_CAMERA_H_

namespace ACFT 
{
	class Camera
	{
	public:
		static inline Camera& GetInstance()
		{
			static Camera g_Camera;
			return g_Camera;
		}

		void UpdateAllVec();

		glm::mat4 GetVP();

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
		Camera() : yaw(PI / 2), pitch(0.0), pos(0.0f, 0.0f, 2.0f), looking(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), right(1.0f, 0.0f, 0.0f) {}
		Camera(const Camera&) = delete;
		Camera(Camera&&) = delete;

		float yaw;
		float pitch;

		glm::vec3 pos;
		glm::vec3 looking;
		glm::vec3 up;
		glm::vec3 right;
	};
}

#endif
