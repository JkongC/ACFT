#ifndef ACFT_VERTEX_H_
#define ACFT_VERTEX_H_

namespace ACFT
{
	struct Vertex
	{
		float position[3];
		float tex_coord[2];
		unsigned int special_tag;

		Vertex& operator+=(const glm::ivec3& pos);
	};

	Vertex operator+(const Vertex& vtx, const glm::vec3& pos);

	class VertexPack
	{
	public:
		inline VertexPack() : vertices() {};
		
		inline constexpr VertexPack(std::initializer_list<Vertex> list)
			: vertices()
		{
			for (const Vertex& vtx : list)
			{
				vertices.at(count++) = vtx;
			}
		}

		void Push(Vertex vertex);

		int GetCount() const;

		const void* GetRawBuffer() const;

		const std::array<Vertex, maxVerteciesPerDraw>& GetBuffer() const;

		void Clear();

	private:
		int count = 0;
		std::array<Vertex, maxVerteciesPerDraw> vertices;
	};
}

#endif
