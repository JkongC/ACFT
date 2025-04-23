module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

module Camera;

import Types;
import Window;

namespace ACFT
{
	OrthographicCamera::OrthographicCamera(Ref<Window> window)
		: m_XPos(window->GetWidth() / 2.0f), m_YPos(window->GetHeight() / 2.0f)
	{

	}

	std::pair<float, float> OrthographicCamera::GetPos() const
	{
		return std::make_pair(m_XPos, m_YPos);
	}

	void OrthographicCamera::SetPos(float x, float y)
	{
		m_XPos = x;
		m_YPos = y;
	}

	void OrthographicCamera::AddPos(float x, float y)
	{
		m_XPos += x;
		m_YPos += y;
	}

	float OrthographicCamera::GetRotateAngle() const
	{
		return m_RotateAngle;
	}

	void OrthographicCamera::SetRotateAngle(float angle)
	{
		m_RotateAngle = angle;
	}

	void OrthographicCamera::AddRotateAngle(float angle)
	{
		m_RotateAngle += angle;
	}

	float OrthographicCamera::GetScale() const
	{
		return m_Scale;
	}

	void OrthographicCamera::SetScale(float scale)
	{
		m_Scale = scale;
	}

	void OrthographicCamera::MultiplyScale(float scale)
	{
		m_Scale *= scale;
	}

	glm::mat4 OrthographicCamera::GetVPMatrix(int window_width, int window_height) const
	{
		auto proj = glm::ortho(m_XPos - window_width / 2.0f, m_XPos + window_width / 2.0f,
			m_YPos - window_height / 2.0f, m_YPos + window_height / 2.0f);
		auto scale = glm::scale(proj, glm::vec3(m_Scale, m_Scale, 1.0f));
		return glm::rotate(scale, m_RotateAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	}
}