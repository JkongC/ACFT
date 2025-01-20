#include "stdafx.h"

#include "Vertex.h"

namespace ACFT
{
	Vertex& Vertex::operator+=(const glm::ivec3& pos)
	{
		this->position[0] += (float)pos.x;
		this->position[1] += (float)pos.y;
		this->position[2] += (float)pos.z;

		return *this;
	}

	Vertex operator+(const Vertex& vtx, const glm::vec3& pos)
	{
		Vertex ret = vtx;
		ret += pos;
		return ret;
	}
}