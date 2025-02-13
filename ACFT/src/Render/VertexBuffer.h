#ifndef ACFT_VERTEXBUFFER_H_
#define ACFT_VERTEXBUFFER_H_

#include "Vertex.h"

namespace ACFT
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		VertexBuffer(const VertexPack& vertices);

		void Bind() const;

		void Unbind() const;

	private:
		unsigned int vb_id;
	};
}

#endif

