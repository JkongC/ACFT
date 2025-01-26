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

		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClearDepth(1.0));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));

		this->ib = std::make_unique<IndexBuffer>();

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

		for (int side = 0; side < block.GetSideCount(); side++)
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

	void BlockRenderer::Flush()
	{
		int count;
		if ((count = vb->GetCount()) != 0){
			Camera& cam = Camera::GetInstance();
			
			shader->SetUniformMat4f("u_MVP", cam.GetVP());
			shader->SetUniformVec3f("u_campos", cam.GetPos());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(Vertex), vb->GetBuffer()));
			GLCall(glDrawElements(GL_TRIANGLES, 6 * sides_should_render, GL_UNSIGNED_INT, nullptr));
			vb->ClearBuffer();
			sides_should_render = 0;
		}
	}


	BackgroundRenderer::BackgroundRenderer()
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));

		this->shader = std::make_unique<Shader>("shader/background.shader");

		shader->Bind();
	}
}