module;

#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module Renderer:OpenGLRenderer;

import <string>;
import <unordered_map>;

import Types;
import Renderer;
import Tesselator;
import Atlas;
import UUID;
import Window;
import Shader;
import Log;
import :GLVertexBuffer;
import :GLVertexArray;
import :GLTexture;
import :GLShader;
import :GLFrameBuffer;

#include "gldbg.h"

namespace ACFT
{
	export constexpr size_t defaultPrimitivePerDraw = 1000;
	
	using namespace GLImplementations;
	export class OpenGLRenderer : public Renderer
	{
	public:
		void DrawTesselator(const Tesselator& tesselator, RenderContext context) override;
		void DrawSprite(const Sprite& sprite, float xpos, float ypos, float width, float height, RenderContext context) override;

		void BeginScene(SceneContext context) override;
		void EndScene() override;

		void SetClearColor(float r, float g, float b, float a) override;
		void SetPrimitive(Primitive primitive) override;

		virtual void SwapWindowFrameBuffers() override;
		virtual void Clear() override;

		virtual void EnableVSync() override;
		virtual void DisableVSync() override;

		virtual void EnableBlend() override;
		virtual void DisableBlend() override;

		RenderAPI GetRenderAPI() override;

		virtual void SetWindowDrawArea(WindowDrawArea area) override;

		OpenGLRenderer();

	protected:
		RenderObjectIdentifier MakeTexture(Ref<Atlas> atlas) override;
		RenderObjectIdentifier MakeTexture(Ref<Image> img) override;

		RenderObjectIdentifier MakeShader(const std::filesystem::path& shader_path, ShaderLang language = ShaderLang::GLSL, ShaderType type = ShaderType::glsl_mixed) override;
		RenderObjectIdentifier MakeBasicShader() override;

		RenderObjectIdentifier MakeFrameBuffer(int width, int height) override;

	private:
		void InitContext();

		template<Primitive primitive>
		void InitBuffers(const VertexBufferLayout& layout)
		{
			IndexBuffer index;
			index.GenerateByMode<primitive>();

			if constexpr (primitive == Primitive::triangle_fan)
			{
				auto [it, success] = this->m_VBOs.try_emplace(Primitive::triangle_fan, 2 + 3 * defaultPrimitivePerDraw * VertexCountPerPrimitive(Primitive::triangle_fan));
				auto& vbo = it->second;
				this->m_VAOs.try_emplace(primitive, layout, vbo, std::move(index));
			}
			else
			{
				auto [it, success] = this->m_VBOs.try_emplace(primitive, 3 * defaultPrimitivePerDraw * VertexCountPerPrimitive(primitive));
				auto& vbo = it->second;
				this->m_VAOs.try_emplace(primitive, layout, vbo, std::move(index));
			}
		}

	private:
		std::unordered_map<Primitive, VertexArray> m_VAOs;
		std::unordered_map<Primitive, VertexBuffer> m_VBOs;

		std::unordered_map<GLuint, GLTexture> m_TextureCache;
		std::unordered_map<GLuint, GLShader> m_ShaderCache;

		std::unordered_map<GLuint, GLFrameBuffer> m_FrameBufferCache;
		
		Primitive m_CurrentPrimitive;

		bool m_UsingBasicShader = false;
	};
}

