module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module Camera;

import <utility>;

import Types;
import Window;

namespace ACFT
{
	export enum class CameraType
	{
		ortho, perspective
	};
	
	export class Camera
	{
	public:
		CameraType GetType() const { return m_Type; }

		virtual glm::mat4 GetVPMatrix(float viewport_width, float viewport_height) const = 0;

	protected:
		Camera(CameraType type) : m_Type(type) {}

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

		glm::mat4 GetVPMatrix(float viewport_width, float viewport_height) const override;

	private:
		float m_XPos = 0.0f;
		float m_YPos = 0.0f;
		float m_RotateAngle = 0.0f;
		float m_Scale = 1.0f;
	};
}