#include "stdafx.h"

#include "ACFT.h"
#include "IndexBuffer.h"

namespace ACFT
{
	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
		:count(count)
	{
		GLCall(glGenBuffers(1, &ib_id));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_id));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
	}

	void IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_id));
	}

	void IndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}