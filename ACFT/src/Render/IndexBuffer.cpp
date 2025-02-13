#include "stdafx.h"

#include "ACFT.h"
#include "IndexBuffer.h"
#include "RenderSystem.h"

namespace ACFT
{
	IndexBuffer::IndexBuffer()
		: count(0), local_buffer({ 0, 1, 2, 2, 3, 0 })
	{
		for (int side = 1; side <= local_buffer.size() / 6 - 1; side++)
		{
			for (int vtx = 0; vtx <= 5; vtx++)
			{
				local_buffer.at(6 * side + vtx) = local_buffer.at(vtx) + 4 * side;
				count++;
			}
		}
		
		RenderSystem::GlGenBuffers(1, &ib_id);
		Bind();
		RenderSystem::GlBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), local_buffer.data(), GL_STATIC_DRAW);
		Unbind();
	}

	IndexBuffer::IndexBuffer(std::initializer_list<unsigned int> template_index, bool fill_to_full)
		: count(0), local_buffer()
	{
		int temp_count = 0;

		for (const unsigned int& idx : template_index)
		{
			local_buffer.at(count++) = idx;
			temp_count++;
		}

		if (fill_to_full)
		{
			int idx_groups = 1;
			
			for (int slot = temp_count; slot < local_buffer.size() - 1; slot += temp_count)
			{
				for (int internal_slot = 0; internal_slot < temp_count; internal_slot++)
				{
					local_buffer.at(slot + internal_slot) = local_buffer.at(internal_slot) + temp_count * idx_groups++;
				}
			}
		}

		RenderSystem::GlGenBuffers(1, &ib_id);
		Bind();
		RenderSystem::GlBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), local_buffer.data(), GL_STATIC_DRAW);
		Unbind();
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