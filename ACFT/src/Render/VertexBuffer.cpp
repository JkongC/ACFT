#include "stdafx.h"

#include "ACFT.h"
#include "VertexBuffer.h"
#include "RenderSystem.h"

namespace ACFT
{
	VertexBuffer::VertexBuffer()
	{
		RenderSystem::GlGenBuffers(1, &vb_id);
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
}