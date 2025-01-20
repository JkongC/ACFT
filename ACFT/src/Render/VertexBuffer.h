#ifndef ACFT_VERTEXBUFFER_H_
#define ACFT_VERTEXBUFFER_H_

#include "Vertex.h"

namespace ACFT
{
	constexpr int maxVerteciesPerDraw = 6 * 300;
	
	class VertexBuffer
	{
	public:
		VertexBuffer();

		void Bind() const;

		void Unbind() const;

		bool PushVertex(Vertex&& vertex);

		void ClearBuffer();

	public:
		inline const int& GetCount() const { return vertex_count; }
		inline Vertex* GetBuffer() { return vertices.data(); }

	private:
		unsigned int vb_id;

		unsigned int vertex_count = 0;
		std::array<Vertex, maxVerteciesPerDraw> vertices;
	};
}

#endif

