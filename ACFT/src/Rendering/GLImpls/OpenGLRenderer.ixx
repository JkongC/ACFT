module;

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <glew.h>
#include <glfw3.h>

export module Renderer:OpenGLRenderer;

import Renderer;
import Tesselator;
import UUID;
import <vector>;
import <string>;
import <unordered_map>;
import Base.glm;
import :GLVertexArray;

namespace ACFT
{
	export constexpr size_t defaultVertexPerDraw = 1000;
	
	using namespace GLImplementations;
	export class OpenGLRenderer : public Renderer
	{
	public:
		void InitContext() override;
		void DrawTesselator(Tesselator tesselator) override;
		void SetClearColor(float r, float g, float b, float a) override;

		RenderAPI GetRenderAPI() override;

		OpenGLRenderer()
			: m_VBO(), m_BasicVAO(), m_DefaultTriangleVAO(), m_DefaultTriangleFanVAO()
			, m_DefaultSquareVAO(), m_DefaultCubeVAO()
		{};

	private:
		std::unordered_map<UUID, VertexArray> m_VAOs;
		VertexBuffer m_VBO;

		UUID m_BasicVAO;
		UUID m_DefaultTriangleVAO;
		UUID m_DefaultTriangleFanVAO;
		UUID m_DefaultSquareVAO;
		UUID m_DefaultCubeVAO;
	};
}

