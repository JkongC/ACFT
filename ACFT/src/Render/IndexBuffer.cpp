#include "stdafx.h"

#include "ACFT.h"
#include "IndexBuffer.h"

namespace ACFT
{
	IndexBuffer::IndexBuffer()
		:count(0), local_buffer({ 0, 1, 2, 1, 2, 3 })
	{
		for (int side = 1; side <= local_buffer.size() / 6 - 1; side++)
		{
			for (int vtx = 0; vtx <= 5; vtx++)
			{
				local_buffer[6 * side + vtx] = local_buffer[vtx] + 4 * side;
				count++;
			}
		}
		
		GLCall(glGenBuffers(1, &ib_id));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_id));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), local_buffer.data(), GL_STATIC_DRAW));
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