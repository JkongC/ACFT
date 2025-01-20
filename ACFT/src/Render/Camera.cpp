#include "stdafx.h"

#include "ACFT.h"
#include "Util/Vec.h"
#include "Camera.h"

namespace ACFT
{
	void Camera::UpdateAllVec()
	{
		this->looking = GetVec3fFromYP(this->yaw, this->pitch);
		if (pitch > 0)
		{
			this->up = GetVec3fFromYP(-this->yaw, PI / 2 - this->pitch);
		}
		else {
			this->up = GetVec3fFromYP(this->yaw, this->pitch + PI / 2);
		}
		this->right = glm::normalize(glm::cross(this->looking, this->up));
	}

	glm::mat4 Camera::GetVP()
	{
		UpdateAllVec();

		glm::mat4 view = glm::lookAt(this->pos, this->pos + this->looking, this->up);
		glm::mat4 perspective = glm::perspective(PI / 4, (float)WindowWidth / (float)WindowHeight, 0.1f, 100.0f);
		return perspective * view;
	}
}