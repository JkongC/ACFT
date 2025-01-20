#ifndef ACFT_VERTEX_H_
#define ACFT_VERTEX_H_

namespace ACFT
{
	struct Vertex
	{
		float position[3];
		float tex_coord[2];

		Vertex& operator+=(const glm::ivec3& pos);
	};

	Vertex operator+(const Vertex& vtx, const glm::vec3& pos);
}

#endif
