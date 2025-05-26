module;

#include <glew.h>
#include <glfw3.h>

#include <entt/entity/registry.hpp>

module Renderer:OpenGLRenderer;

import Types;
import Tesselator;
import UUID;
import Log;
import Window;
import Camera;
import Event;
import LockfreeQueue;
import :GLVertexArray;
import :GLIndexBuilder;
import :GLTexture;
import :GLShader;
import ACFT.EnhancingFuncs;

#include "gldbg.h"

namespace ACFT
{	
	using namespace GLImplementations;

	OpenGLRenderer::OpenGLRenderer()
		: m_CurrentPrimitive(cube)
	{
		InitContext();
	}
	
	void OpenGLRenderer::DrawTesselator(const Tesselator& tesselator, RenderContext context)
	{
		bool immediate_draw = false;
		
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
		
		if (context.shader == nullptr && !m_UsingBasicShader)
		{
			m_RenderContextCache.shader = ShaderLib::GetBasicShader();
			m_UsingBasicShader = true;
			immediate_draw = true;
		}
		else if (context.shader != nullptr && context.shader != m_RenderContextCache.shader)
		{
			m_RenderContextCache.shader = context.shader;
			m_UsingBasicShader = false;
			immediate_draw = true;
		}

		GLuint shader_id;
		RenderObjectIdentifier _id = m_RenderContextCache.shader->GetIdentifier();
		shader_id = VariantHelper::ValueOr<unsigned int>(_id, 0);

		if (auto it = m_ShaderCache.find(shader_id); 
			it != m_ShaderCache.end())
		{
			auto& [id, shader] = *it;
			shader.Bind();

			shader.SetUniformMat4f("mvp", m_SceneContext.camera->GetVPMatrix(m_Window->GetWidth(), m_Window->GetHeight()));
		}

		auto& vertices = tesselator.GetVertices();
		for (const Scope<Vertex>& vtx : vertices)
		{
			while (immediate_draw || !vbo.Submit(*vtx))
			{
				GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vbo.GetCurrentVertexCount() / VertexCountPerPrimitive(tesselator.GetMode()) * IndexCountPerPrimitive(tesselator.GetMode())),
					GL_UNSIGNED_INT, nullptr));

				vbo.Clear();
				immediate_draw = false;
			}
		}
	}

	void OpenGLRenderer::DrawSprite(const Sprite& sprite, float xpos, float ypos, float width, float height, RenderContext context)
	{
		auto& texture = sprite.GetCurrentImage();
		if (auto* idptr = std::get_if<unsigned int>(&texture.m_Identifier))
		{
			unsigned int atlas_texture_id = *idptr;

			GLTexture::Bind(atlas_texture_id);
			Tesselator tesselator{ Primitive::square };

			auto& uv = texture.info.uv_coords;

			tesselator.NewVertex()
				.Pos(xpos, ypos, 0.0f)
				.UVCoords(uv.min_u, uv.max_v);
			tesselator.NewVertex()
				.Pos(xpos, ypos - height, 0.0f)
				.UVCoords(uv.min_u, uv.min_v);
			tesselator.NewVertex()
				.Pos(xpos + width, ypos - height, 0.0f)
				.UVCoords(uv.max_u, uv.min_v);
			tesselator.NewVertex()
				.Pos(xpos + width, ypos, 0.0f)
				.UVCoords(uv.max_u, uv.max_v);

			DrawTesselator(tesselator, context);
		}
		else
		{
			ACFT_LOG_WARN("[Renderer] Trying to draw a sprite without a texture!");
		}
			
	}

	RenderAPI OpenGLRenderer::GetRenderAPI()
	{
		return RenderAPI::OpenGL;
	}

	void OpenGLRenderer::InitContext()
	{
		VertexBufferLayout basic_layout;
		basic_layout.Push<float>(3); // position
		basic_layout.Push<float>(2); // uv coords
		
		InitBuffers<Primitive::point>(basic_layout);
		InitBuffers<Primitive::triangle>(basic_layout);
		InitBuffers<Primitive::triangle_fan>(basic_layout);
		InitBuffers<Primitive::square>(basic_layout);
		InitBuffers<Primitive::cube>(basic_layout);
	}

	void OpenGLRenderer::BeginScene(SceneContext context)
	{
		m_SceneContext = context;
		Clear();
	}

	void OpenGLRenderer::EndScene()
	{
		for (auto& [primitive, vao] : m_VAOs)
		{
			auto& [p, vbo] = *this->m_VBOs.find(primitive);
			vao.Bind();
			vbo.Bind();
			
			GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vbo.GetCurrentVertexCount() / VertexCountPerPrimitive(primitive) * IndexCountPerPrimitive(primitive)),
				GL_UNSIGNED_INT, nullptr));

			vbo.Clear();
		}

		if (auto event_opt = m_EventQueue.Pop())
		{
			Ref<Event> event = event_opt.value();
			if (auto* info = event->GetInfo<WindowSizeInfo>())
			{
				GLCall(glViewport(0, 0, info->width, info->height));
			}
		}

		SwapWindowFrameBuffers();
	}

	void OpenGLRenderer::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void OpenGLRenderer::SetPrimitive(Primitive primitive)
	{
		this->m_CurrentPrimitive = primitive;
	}

	void OpenGLRenderer::SwapWindowFrameBuffers()
	{
		m_Window->SwapFrameBuffers();
	}

	void OpenGLRenderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRenderer::EnableVSync()
	{
		glfwSwapInterval(1);
	}

	void OpenGLRenderer::DisableVSync()
	{
		glfwSwapInterval(0);
	}

	void OpenGLRenderer::EnableBlend()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRenderer::DisableBlend()
	{
		glDisable(GL_BLEND);
	}

	void OpenGLRenderer::SetWindowDrawArea(WindowDrawArea area)
	{
		if (area == WindowDrawArea::user)
		{
			UserAreaRect area = m_Window->GetUserArea();
			int window_width = m_Window->GetWidth();
			int window_height = m_Window->GetHeight();

			int x, y, width, height;
			if (area.use_percentage)
			{
				x = static_cast<int>(window_width * area.left);
				y = static_cast<int>(window_height * area.bottom);
				width = static_cast<int>(window_width - window_width * area.right - x);
				height = static_cast<int>(window_height - window_height * area.top - y);
			}
			else
			{
				x = static_cast<int>(area.left);
				y = static_cast<int>(area.bottom);
				width = static_cast<int>(window_width - x - area.right);
				height = static_cast<int>(window_height - y - area.top);
			}

			glViewport(x, y, width, height);
		}
		else
		{
			glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
		}
	}

	RenderObjectIdentifier OpenGLRenderer::MakeTexture(Ref<Atlas> atlas)
	{
		Scope<GLTexture> texture = MakeScope<GLTexture>(atlas);
		unsigned int id = texture->GetID();
		m_TextureCache.try_emplace(id, std::move(*texture));
		return id;
	}

	RenderObjectIdentifier OpenGLRenderer::MakeTexture(Ref<Image> img)
	{
		Scope<GLTexture> texture = MakeScope<GLTexture>(img);
		unsigned int id = texture->GetID();
		m_TextureCache.try_emplace(id, std::move(*texture));
		return id;
	}

	RenderObjectIdentifier OpenGLRenderer::MakeShader(const std::filesystem::path& shader_path, ShaderLang language, ShaderType type)
	{
		Scope<GLShader> shader = MakeScope<GLShader>(shader_path);
		unsigned int id = VariantHelper::Value<unsigned int>(shader->GetIdentifier());
		m_ShaderCache.try_emplace(id, std::move(*shader));
		return id;
	}

	RenderObjectIdentifier OpenGLRenderer::MakeBasicShader()
	{
		Scope<GLShader> shader = MakeScope<GLShader>();
		unsigned int id = VariantHelper::Value<unsigned int>(shader->GetIdentifier());
		m_ShaderCache.try_emplace(id, std::move(*shader));
		return id;
	}

	RenderObjectIdentifier OpenGLRenderer::MakeFrameBuffer(int width, int height)
	{
		Scope<GLFrameBuffer> fbo = MakeScope<GLFrameBuffer>(width, height);
		unsigned int id = fbo->GetID();
		m_FrameBufferCache.try_emplace(id, std::move(*fbo));
		return id;
	}
}