module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

module Camera;

import Renderer;

namespace ACFT
{
	Camera::Camera(CameraType type)
		: m_Type(type)
	{
		auto callback = [this](Ref<Event> event) -> void { this->OnEvent(event); };
		Ref<Camera> self_ref = RefFromThis();
		auto& event_manager = EventManager::Global();
		for (auto& type : EventRegistry::GetAllTypes())
		{
			event_manager.Subscribe(self_ref, type, callback);
		}
	}
	
	OrthographicCamera::OrthographicCamera()
		: Camera(CameraType::ortho), m_XPos(0.0f), m_YPos(0.0f)
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

	glm::mat4 OrthographicCamera::GetVPMatrix(float viewport_width, float viewport_height) const
	{
		auto proj = glm::ortho(m_XPos, m_XPos + viewport_width,
			m_YPos, m_YPos + viewport_height);
		auto scale = glm::scale(proj, glm::vec3(m_Scale, m_Scale, 1.0f));
		return glm::rotate(scale, m_RotateAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	std::pair<float, float> OrthographicCamera::WindowPosToWorldPos(Ref<Window>& window, float xpos, float ypos)
	{
		if (window->BorderCustomized())
		{
			auto& user_area = window->GetUserArea();
			float left, top, bottom;

			if (user_area.use_percentage)
			{
				left = user_area.left * window->GetWidth();
				top = user_area.top * window->GetHeight();
				bottom = (1.0f - user_area.bottom) * window->GetHeight();
			}
			else
			{
				left = user_area.left;
				top = user_area.top;
				bottom = window->GetHeight() - user_area.bottom;
			}

			return { m_XPos + (xpos - left) / m_Scale, m_YPos + (bottom - ypos) / m_Scale };
		}
		else
		{
			return { m_XPos + xpos / m_Scale, m_YPos + (window->GetHeight() - ypos) / m_Scale };
		}
	}

	void OrthographicCamera::OnEvent(Ref<Event> event)
	{

	}

	PerspectiveCamera::PerspectiveCamera()
		: Camera(CameraType::perspective)
	{
	}

	void PerspectiveCamera::SetPos(glm::vec3 pos)
	{
		m_Pos = pos;
	}

	void PerspectiveCamera::SetFOV(float fov)
	{
		m_FOV = fov;
	}

	void PerspectiveCamera::SetYaw(float yaw)
	{
		m_Yaw = yaw;
	}

	void PerspectiveCamera::SetPitch(float pitch)
	{
		m_Pitch = pitch;
	}

	glm::mat4 PerspectiveCamera::GetVPMatrix(float viewport_width, float viewport_height) const
	{
		auto& window = Renderer::GetWindow();
		float aspect = static_cast<float>(window->GetWidth()) / window->GetHeight();
		return glm::perspective(m_FOV, aspect, 0.1f, 100.0f);
	}
}