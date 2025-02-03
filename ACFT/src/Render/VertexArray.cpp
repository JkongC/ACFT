#include "stdafx.h"

#include "ACFT.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

namespace ACFT
{
	VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &va_id));
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(va_id));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const
	{
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();
		unsigned offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.count, element.type,
				element.normalized, layout.GetStride(), (const void*)offset));
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
	}
}


