#include "stdafx.h"

#include "ACFT.h"
#include "VertexBuffer.h"

namespace ACFT
{
	VertexBuffer::VertexBuffer()
		:vertices()
	{
		GLCall(glGenBuffers(1, &vb_id));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb_id));
		GLCall(glBufferData(GL_ARRAY_BUFFER, maxVerteciesPerDraw * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb_id));
	}

	void VertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	bool VertexBuffer::PushVertex(Vertex vertex)
	{
		if (vertex_count == maxVerteciesPerDraw)
			return false;
		vertices[vertex_count++] = vertex;
		return true;
	}

	void VertexBuffer::ClearBuffer()
	{
		vertex_count = 0;
	}
}