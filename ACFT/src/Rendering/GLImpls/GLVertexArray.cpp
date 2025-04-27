module;

#include <glew.h>
#include <glfw3.h>

module Renderer:GLVertexArray;

import Types;

import Log;

#include "gldbg.h"

namespace GLImplementations
{
	using namespace ACFT;
	VertexArray::VertexArray(const VertexBufferLayout& layout, const VertexBuffer& vbo, IndexBuffer&& ibo)
		: m_IBO(MakeRef<IndexBuffer>(std::move(ibo)))
	{
		glGenVertexArrays(1, &m_ArrayID);
		Init(vbo, layout);
	}

	VertexArray::VertexArray(const VertexBufferLayout& layout, const Ref<VertexBuffer>& vbo, const Ref<IndexBuffer>& ibo)
		: m_IBO(ibo)
	{
		glGenVertexArrays(1, &m_ArrayID);
		Init(*vbo, layout);
	}

	void VertexArray::Init(const VertexBuffer& vbo, const VertexBufferLayout& layout) const
	{
		Bind();
		vbo.Bind();
		m_IBO->Bind();
		const auto& elements = layout.GetElements();
		uintptr_t offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, static_cast<GLsizei>(element.count), element.type,
				element.normalized, static_cast<GLsizei>(layout.GetStride()), reinterpret_cast<const void*>(offset)));
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
		Unbind();
		vbo.Unbind();
		m_IBO->Unbind();
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_ArrayID);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_ArrayID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}