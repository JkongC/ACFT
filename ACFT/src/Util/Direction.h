#ifndef ACFT_DIRECTION_H_
#define ACFT_DIRECTION_H_

namespace ACFT
{
	namespace Direction
	{
		const glm::vec3 front(0.0f, 0.0f, 1.0f);
		const glm::vec3 back(0.0f, 0.0f, -1.0f);
		const glm::vec3 left(-1.0f, 0.0f, 0.0f);
		const glm::vec3 right(1.0f, 0.0f, 0.0f);
		const glm::vec3 up(0.0f, 1.0f, 0.0f);
		const glm::vec3 down(0.0f, -1.0f, 0.0f);
	}
}

#endif
