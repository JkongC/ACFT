module;

#include <glew.h>
#include <glfw3.h>

module Renderer:GLVertexArray;

import <memory>;

namespace GLImplementations
{
	VertexArray::VertexArray(const VertexBufferLayout& layout, IndexBuffer&& ibo)
		: m_IBO(std::make_shared<IndexBuffer>(std::move(ibo)))
	{
		glGenVertexArrays(1, &m_ArrayID);
		Init(layout);
	}

	VertexArray::VertexArray(const VertexBufferLayout& layout, const std::shared_ptr<IndexBuffer>& ibo)
		: m_IBO(ibo)
	{
		glGenVertexArrays(1, &m_ArrayID);
		Init(layout);
	}

	void VertexArray::Init(const VertexBufferLayout& layout) const
	{
		Bind();
		m_IBO->Bind();
		const auto& elements = layout.GetElements();
		uintptr_t offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, static_cast<GLsizei>(element.count), element.type,
				element.normalized, static_cast<GLsizei>(layout.GetStride()), reinterpret_cast<const void*>(offset));
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
		Unbind();
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