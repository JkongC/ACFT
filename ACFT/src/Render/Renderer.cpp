#include "stdafx.h"

#include "ACFT.h"
#include "Voxel.h"
#include "Renderer.h"
#include "Log/Logger.h"

namespace ACFT
{
	BlockRenderer::BlockRenderer()
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		unsigned int index[36] = {
			0, 1, 2, 
			1, 2, 3
		};

		for (int i = 1; i <= 5; i++)
		{
			for (int j = 0; j <= 5; j++)
			{
				index[6 * i + j] = index[j] + 4 * i;
			}
		}

		this->ib = std::make_unique<IndexBuffer>(index, 36);

		this->vb = std::make_unique<VertexBuffer>();

		this->va = std::make_unique<VertexArray>();

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		va->AddBuffer(*vb, layout);

		this->shader = std::make_unique<Shader>("shader/basic.shader");

		shader->Bind();
		va->Bind();
		ib->Bind();
	}

	void BlockRenderer::Draw(const Block& block)
	{
		const auto& vertices = block.GetVoxel().GetVertices();

		block.GetVoxel().GetTexture().Bind();
		shader->Bind();
		va->Bind();
		ib->Bind();
		vb->Bind();

		const auto& render_flag = block.GetRenderFlag();

		for (int side = 0; side < 6; side++)
		{
			if (render_flag[side])
			{
				if (vb->GetCount() + 4 > maxVerteciesPerDraw)
					Flush();
				
				for (int vertex = 0; vertex < 4; vertex++)
				{
					vb->PushVertex(vertices.vertices[4 * side + vertex] + block.GetPos());
				}
				sides_should_render++;
			}
		}
	}

	void BlockRenderer::Flush()
	{
		int count;
		if ((count = vb->GetCount()) != 0){
			shader->SetUniformMat4f("u_MVP", Camera::GetInstance().GetVP());
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(Vertex), vb->GetBuffer()));
			GLCall(glDrawElements(GL_TRIANGLES, 6 * sides_should_render, GL_UNSIGNED_INT, nullptr));
			vb->ClearBuffer();
		}
	}
}