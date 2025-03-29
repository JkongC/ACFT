module;

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <glew.h>
#include <glfw3.h>
#include <malloc.h>

module Renderer:GLIndexBuffer;

import <array>;
import <functional>;
import <unordered_map>;
import Tesselator;
import :GLIndexBuilder;

namespace GLImplementations
{
	using namespace ACFT;

#define mp(e1, e2) std::make_pair<Primitive, VertexExpander>(e1, e2)

	IndexBuffer::IndexBuffer()
	{
		glGenBuffers(1, &m_BufferID);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	template<Primitive mode>
	void IndexBuffer::GenerateByMode(unsigned int max_primitive_count)
	{	
		Bind();
		std::vector<unsigned int> indices = IndexBuilder::Create(mode)
			.IndexCount(IndexCountPerPrimitive(mode) * max_primitive_count)
			.Build();
		this->m_Count = indices.size();
		this->m_Size = this->m_Count * sizeof(unsigned int);

		unsigned int* buffer = static_cast<unsigned int*>(alloca(this->m_Size));

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_Size, buffer, GL_STATIC_DRAW);

		Unbind();
	}

	template<Primitive mode>
	void IndexBuffer::GenerateByMode()
	{
		Bind();
		std::vector<unsigned int> indices = IndexBuilder::GetDefaultBuild(mode);
		this->m_Count = indices.size();
		this->m_Size = this->m_Count * sizeof(unsigned int);

		unsigned int* buffer = static_cast<unsigned int*>(alloca(this->m_Size));

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_Size, buffer, GL_STATIC_DRAW);

		Unbind();
	}
}
