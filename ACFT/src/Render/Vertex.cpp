#include "stdafx.h"

#include "ACFT.h"
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

	void VertexPack::Push(Vertex vertex)
	{
		if (count < maxVerteciesPerDraw)
			vertices.at(count++) = vertex;
	}

	int VertexPack::GetCount() const
	{
		return count;
	}

	const void* VertexPack::GetRawBuffer() const
	{
		return vertices.data();
	}

	std::array<Vertex, maxVerteciesPerDraw>& VertexPack::GetBuffer()
	{
		return vertices;
	}

	void VertexPack::Clear()
	{
		count = 0;
	}
}