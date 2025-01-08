#include "stdafx.h"

#include "ACFT.h"
#include "Renderer.h"
#include "Log/Logger.h"

namespace ACFT
{
	Renderer::Renderer()
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		unsigned int index[6] = {
			0, 1, 2, 
			1, 2, 3
		};

		this->ib = std::make_unique<IndexBuffer>(index, 6);

		this->vb = std::make_unique<VertexBuffer>(1000);

		this->va = std::make_unique<VertexArray>();

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		va->AddBuffer(*vb, layout);

		this->shader = std::make_unique<Shader>("shader/basic.shader");
	}

	void Renderer::Draw()
	{
		shader->Bind();
		va->Bind();
		ib->Bind();
		
		GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::Flush()
	{

	}
}