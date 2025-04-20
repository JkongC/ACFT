module;

#include <glew.h>
#include <glfw3.h>

#include <entt/entity/registry.hpp>

module Renderer:GLVertexBuffer;

import Log;
import Vertex;

namespace GLImplementations
{
	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &m_BufferID);
	}
	
	VertexBuffer::VertexBuffer(size_t max_size)
		: m_MaxBufferSize(max_size)
	{
		glGenBuffers(1, &m_BufferID);
		glBufferData(GL_ARRAY_BUFFER, m_MaxBufferSize, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void VertexBuffer::Init(size_t max_size)
	{
		m_MaxBufferSize = max_size;
		glBufferData(GL_ARRAY_BUFFER, m_MaxBufferSize, nullptr, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	bool VertexBuffer::Submit(const ACFT::Vertex& vtx)
	{
		if (vtx.GetSize() > GetRemainingSize())
			return false;

		PushSingleAttribute<ACFT::VertexPos>(vtx);
		PushSingleAttribute<ACFT::VertexColorRGB>(vtx);
		PushSingleAttribute<ACFT::VertexColorRGBA>(vtx);
		PushSingleAttribute<ACFT::VertexTexture>(vtx);

		this->m_CurrentVertexCount++;
		return true;
	}

	void VertexBuffer::Clear()
	{
		this->m_CurrentSize = 0;
		this->m_CurrentVertexCount = 0;
	}
}