module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module Camera;

import <utility>;

import Types;
import Event;
import Window;

namespace ACFT
{
	export enum class CameraType
	{
		ortho, perspective
	};
	
	export class Camera : public EnableRefFromThis<Camera>
	{
	public:
		CameraType GetType() const { return m_Type; }

		virtual glm::mat4 GetVPMatrix(float viewport_width, float viewport_height) const = 0;

		virtual void OnEvent(Ref<Event> event) = 0;

	protected:
		Camera(CameraType type);

	protected:
		CameraType m_Type;
	};
	
	export class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera();

		std::pair<float, float> GetPos() const;
		void SetPos(float x, float y);
		void AddPos(float x, float y);

		float GetRotateAngle() const;
		void SetRotateAngle(float angle);
		void AddRotateAngle(float angle);

		float GetScale() const;
		void SetScale(float scale);
		void MultiplyScale(float scale);

		std::pair<float, float> WindowPosToWorldPos(Ref<Window>& window, float xpos, float ypos);

		virtual glm::mat4 GetVPMatrix(float viewport_width, float viewport_height) const override;

		virtual void OnEvent(Ref<Event> event) override;

	private:
		float m_XPos = 0.0f;
		float m_YPos = 0.0f;
		float m_RotateAngle = 0.0f;
		float m_Scale = 1.0f;
	};

	export class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera();

		glm::vec3 GetPos() const { return m_Pos; }
		void SetPos(glm::vec3 pos);

		float GetFOV() const { return m_FOV; }
		void SetFOV(float fov);

		float GetYaw() const { return m_Yaw; }
		void SetYaw(float yaw);

		float GetPitch() const{ return m_Pitch; }
		void SetPitch(float pitch);

		virtual glm::mat4 GetVPMatrix(float viewport_width, float viewport_height) const override;

	private:
		glm::vec3 m_Pos = { 0.0f, 0.0f, 0.0f };
		float m_FOV = 1.0;
		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;
	};
}