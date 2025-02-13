#include "stdafx.h"

#include "ACFT.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "RenderSystem.h"

namespace ACFT
{
	VertexArray::VertexArray()
	{
		RenderSystem::GlGenVertexArrays(1, &va_id);
	}

	void VertexArray::Bind() const
	{
		RenderSystem::GlBindVertexArray(va_id);
	}

	void VertexArray::Unbind() const
	{
		RenderSystem::GlBindVertexArray(0);
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, const IndexBuffer& ibo) const
	{
		Bind();
		vb.Bind();
		ibo.Bind();
		const auto& elements = layout.GetElements();
		uintptr_t offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			RenderSystem::GlEnableVertexAttribArray(i);
			RenderSystem::GlVertexAttribPointer(i, element.count, element.type,
				element.normalized, layout.GetStride(), reinterpret_cast<const void*>(offset));
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
		Unbind();
		vb.Unbind();
		ibo.Unbind();
	}
}


