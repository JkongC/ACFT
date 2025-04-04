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
	export constexpr size_t defaultPrimitivePerDraw = 1000;
	
	using namespace GLImplementations;
	export class OpenGLRenderer : public Renderer
	{
	public:
		void InitContext() override;
		void DrawTesselator(const Tesselator& tesselator) override;
		void DrawSprite(const Sprite& sprite) override;

		void BeginScene() override;
		void EndScene() override;

		void SetClearColor(float r, float g, float b, float a) override;
		void SetPrimitive(Primitive primitive) override;

		RenderAPI GetRenderAPI() override;

		OpenGLRenderer()
			: m_CurrentPrimitive(cube)
		{};

	private:
		template<Primitive primitive>
		void InitBuffers(const VertexBufferLayout& layout);

	private:
		std::unordered_map<Primitive, VertexArray> m_VAOs;
		std::unordered_map<Primitive, VertexBuffer> m_VBOs;

		Primitive m_CurrentPrimitive;
	};
}

