module;

#include <glew.h>
#include <glfw3.h>

module Renderer:OpenGLRenderer;

import Tesselator;
import UUID;
import Log;
import :GLVertexArray;
import :GLIndexBuilder;

namespace ACFT
{	
	using namespace GLImplementations;

	void OpenGLRenderer::DrawTesselator(const Tesselator& tesselator)
	{
		auto vao_it = this->m_VAOs.find(tesselator.GetMode());
		if (vao_it == this->m_VAOs.end())
		{
			ACFT_LOG_ERROR("[GLRenderer] {} VAO uninitialized!", PrimitiveToString(tesselator.GetMode()));
			return;
		}
		auto vbo_it = this->m_VBOs.find(tesselator.GetMode());
		if (vbo_it == this->m_VBOs.end())
		{
			ACFT_LOG_ERROR("[GLRenderer] {} VBO uninitialized!", PrimitiveToString(tesselator.GetMode()));
			return;
		}

		VertexArray& vao = vao_it->second;
		VertexBuffer& vbo = vbo_it->second;

		vao.Bind();
		vbo.Bind();

		auto& vertices = tesselator.GetVertices();
		for (const Vertex& vtx : vertices)
		{
			while (!vbo.Submit(vtx))
			{
				glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vbo.GetCurrentVertexCount() / VertexCountPerPrimitive(tesselator.GetMode()) * IndexCountPerPrimitive(tesselator.GetMode())),
					GL_UNSIGNED_INT, nullptr);
			}
		}
	}

	void OpenGLRenderer::DrawSprite(const Sprite& sprite)
	{

	}

	RenderAPI OpenGLRenderer::GetRenderAPI()
	{
		return RenderAPI::OpenGL;
	}

	void OpenGLRenderer::InitContext()
	{
		VertexBufferLayout basic_layout;
		basic_layout.Push<float>(3);
		
		InitBuffers<Primitive::point>(basic_layout);
		InitBuffers<Primitive::triangle>(basic_layout);
		InitBuffers<Primitive::triangle_fan>(basic_layout);
		InitBuffers<Primitive::square>(basic_layout);
		InitBuffers<Primitive::cube>(basic_layout);
	}

	void OpenGLRenderer::BeginScene()
	{

	}

	void OpenGLRenderer::EndScene()
	{
		for (auto& [primitive, vao] : m_VAOs)
		{
			auto& [p, vbo] = *this->m_VBOs.find(primitive);
			vao.Bind();
			vbo.Bind();

			glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vbo.GetCurrentVertexCount() / VertexCountPerPrimitive(primitive) * IndexCountPerPrimitive(primitive)),
				GL_UNSIGNED_INT, nullptr);
		}
	}

	void OpenGLRenderer::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void OpenGLRenderer::SetPrimitive(Primitive primitive)
	{
		this->m_CurrentPrimitive = primitive;
	}
}