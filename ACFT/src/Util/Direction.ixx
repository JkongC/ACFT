module;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module Direction;

export namespace ACFT
{
	namespace Direction
	{
		inline const glm::vec3 front(0.0f, 0.0f, 1.0f);
		inline const glm::vec3 back(0.0f, 0.0f, -1.0f);
		inline const glm::vec3 left(-1.0f, 0.0f, 0.0f);
		inline const glm::vec3 right(1.0f, 0.0f, 0.0f);
		inline const glm::vec3 up(0.0f, 1.0f, 0.0f);
		inline const glm::vec3 down(0.0f, -1.0f, 0.0f);
	}
}