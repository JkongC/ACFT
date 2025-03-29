export module Camera;

import Base.glm;

namespace ACFT
{
	export class Camera
	{
	public:
		Camera() = default;

	private:
		glm::vec3 m_Looking{0.0f, 0.0f, -1.0f};
		glm::vec3 m_Up{0.0f, 1.0f, 0.0f};
		glm::vec3 m_Right{1.0f, 0.0f, 0.0f};
	};
}