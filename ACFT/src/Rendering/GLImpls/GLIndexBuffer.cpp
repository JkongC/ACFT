module;

#include <glew.h>
#include <glfw3.h>

module Renderer:GLIndexBuffer;

import Log;

#include "gldbg.h"

namespace GLImplementations
{
	using namespace ACFT;

	IndexBuffer::IndexBuffer()
	{
		GLCall(glGenBuffers(1, &m_BufferID));
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_BufferID));
	}

	void IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
	}

	void IndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}
