module;

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <glew.h>
#include <glfw3.h>

module Renderer:OpenGLRenderer;

import Tesselator;
import UUID;
import :GLVertexArray;

namespace ACFT
{	
	using namespace GLImplementations;

	void OpenGLRenderer::DrawTesselator(Tesselator tesselator)
	{

	}

	RenderAPI OpenGLRenderer::GetRenderAPI()
	{
		return RenderAPI::OpenGL;
	}

	void OpenGLRenderer::InitContext()
	{
		this->m_VBO.Bind();
		glBufferData(GL_ARRAY_BUFFER, 96000, nullptr, GL_DYNAMIC_DRAW);

		VertexBufferLayout basic_layout;
		basic_layout.Push<float>(3);
		
		IndexBuffer basic_index;
		basic_index.GenerateByMode<Primitive::point>();
		this->m_VAOs.try_emplace(m_BasicVAO, basic_layout, std::move(basic_index));
		
		IndexBuffer triangle_index;
		triangle_index.GenerateByMode<Primitive::triangle>();
		this->m_VAOs.try_emplace(m_DefaultTriangleVAO, basic_layout, std::move(triangle_index));

		IndexBuffer triangle_fan_index;
		triangle_fan_index.GenerateByMode<Primitive::triangle>();
		this->m_VAOs.try_emplace(m_DefaultTriangleFanVAO, basic_layout, std::move(triangle_fan_index));

		IndexBuffer square_index;
		square_index.GenerateByMode<Primitive::square>();
		this->m_VAOs.try_emplace(m_DefaultTriangleVAO, basic_layout, std::move(square_index));

		IndexBuffer cube_index;
		cube_index.GenerateByMode<Primitive::cube>();
		this->m_VAOs.try_emplace(m_DefaultTriangleVAO, basic_layout, std::move(cube_index));
	}

	void OpenGLRenderer::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}
}