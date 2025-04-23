module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module Camera;

import Types;
import Window;

namespace ACFT
{
	export class Camera
	{
	protected:
		Camera() = default;
	};
	
	export class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera() = default;
		OrthographicCamera(Ref<Window> window);

		std::pair<float, float> GetPos() const;
		void SetPos(float x, float y);
		void AddPos(float x, float y);

		float GetRotateAngle() const;
		void SetRotateAngle(float angle);
		void AddRotateAngle(float angle);

		float GetScale() const;
		void SetScale(float scale);
		void MultiplyScale(float scale);

		glm::mat4 GetVPMatrix(int window_width, int window_height) const;

	private:
		float m_XPos = 0.0f;
		float m_YPos = 0.0f;
		float m_RotateAngle = 0.0f;
		float m_Scale = 1.0f;
	};
}