#include "stdafx.h"

#include "ACFT.h"
#include "Log/Logger.h"
#include "RenderSystem.h"

#define RENDERCALL(function, ...) \
if (!Game::IsRenderThread()) \
	{RecordDrawCall([__VA_ARGS__]() -> void {GLCall(function);});}\
else \
	{GLCall(function);}

#define NORMALCALL(functions, ...) \
if (!Game::IsRenderThread()) \
	{RecordDrawCall([__VA_ARGS__]() -> void {functions});}\
else \
	{functions}

namespace ACFT
{
	static RenderSystem* rs_instance = nullptr;

	void RenderSystem::Init()
	{
		if (!Game::IsRenderThread())
		{
			ACFT_LOG_ERROR("Trying to initialize the render system from non-render thread!");
		}
		else
		{
			if (rs_instance == nullptr)
				rs_instance = new RenderSystem();
		}
	}

	void RenderSystem::Shutdown()
	{
		if (rs_instance != nullptr)
		{
			delete rs_instance;
			rs_instance = nullptr;
		}
	}

	RenderSystem& RenderSystem::GetInstance()
	{
		if (rs_instance == nullptr)
			Init();
		return *rs_instance;
	}

	void RenderSystem::StartFrame()
	{
		auto& cmd_buffer = GetInstance().command_buffer;
		cmd_buffer = new std::vector<RenderCommand>();
		cmd_buffer->reserve(10);

		glm::mat4 mvp = Camera::GetInstance().GetVP();
		glm::vec3 campos = Camera::GetInstance().GetPos();
		NORMALCALL(
			GLCall(glUniformMatrix4fv(GetInstance().global_shader.GetUniformLocation("u_MVP"), 1, GL_FALSE, &mvp[0][0]));
			GLCall(glUniform3fv(GetInstance().global_shader.GetUniformLocation("u_campos"), 1, &campos.x));
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		, =
			)
	}

	void RenderSystem::FlushFrame()
	{
		NORMALCALL(
			GetInstance().varray_list[VertexArrayType::normal].Bind();
		VertexPack & vertices = GetInstance().local_vertex_buffer;
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.GetCount() * sizeof(Vertex), vertices.GetRawBuffer()));
		GLCall(glDrawElements(GL_TRIANGLES, vertices.GetCount() * 1.5, GL_UNSIGNED_INT, nullptr));
		vertices.Clear();
			)
	}

	void RenderSystem::EndFrame()
	{
		FlushFrame();
		RENDERCALL(glfwSwapBuffers(Game::GetGameWindow()));
		auto& cmd_buffer = *GetInstance().command_buffer;
		GetInstance().render_queue.PushCommand(MakeRef<RenderCommand>(std::move([cmds = std::move(cmd_buffer)]() -> void
			{
				for (const auto& cmd : cmds)
				{
					cmd();
				}
			})));
	}

	void RenderSystem::PushVertex(const Ref<VertexPack>& _vertices)
	{
		NORMALCALL(
			VertexPack & vertices = GetInstance().local_vertex_buffer;

		if (_vertices->GetCount() > maxVerteciesPerDraw - vertices.GetCount())
		{
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.GetCount() * sizeof(Vertex), vertices.GetRawBuffer()));
			GLCall(glDrawElements(GL_TRIANGLES, vertices.GetCount() * sizeof(Vertex), GL_UNSIGNED_INT, nullptr));
			vertices.Clear();
		}

		auto& buffer = _vertices->GetBuffer();
		for (int i = 0; i < _vertices->GetCount(); i++)
		{
			vertices.Push(buffer[i]);
		}
		, =
			)
	}

	int RenderSystem::GetCurrentVertexCount()
	{
		return GetInstance().local_vertex_buffer.GetCount();
	}

	bool RenderSystem::PollCommand()
	{
		if (Game::IsRenderThread())
		{
			auto command = GetInstance().render_queue.FetchCommand();
			if (command)
			{
				(*command.value())();
				return true;
			}
		}

		return false;
	}

	void RenderSystem::RecordDrawCall(const RenderCommand& draw_call)
	{
		GetInstance().command_buffer->push_back(draw_call);
	}

	void RenderSystem::GlGenBuffers(GLsizei count, GLuint* id)
	{
		RENDERCALL(glGenBuffers(count, id), =)
	}

	void RenderSystem::GlBindBuffer(GLenum target, GLuint buffer)
	{
		RENDERCALL(glBindBuffer(target, buffer), =)
	}

	void RenderSystem::GlBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
	{
		RENDERCALL(glBufferData(target, size, data, usage), =)
	}

	void RenderSystem::GlBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
	{
		RENDERCALL(glBufferSubData(target, offset, size, data), =)
	}

	void RenderSystem::GlGenVertexArrays(GLsizei n, GLuint* arrays)
	{
		RENDERCALL(glGenVertexArrays(n, arrays), =)
	}

	void RenderSystem::GlBindVertexArray(GLuint array)
	{
		RENDERCALL(glBindVertexArray(array), =)
	}

	void RenderSystem::GlEnableVertexAttribArray(GLuint index)
	{
		RENDERCALL(glEnableVertexAttribArray(index), =)
	}

	void RenderSystem::GlVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
	{
		RENDERCALL(glVertexAttribPointer(index, size, type, normalized, stride, pointer), =)
	}

	void RenderSystem::GlGenTextures(GLsizei n, GLuint* textures)
	{
		RENDERCALL(glGenTextures(n, textures), =)
	}

	void RenderSystem::GlBindTexture(GLenum target, GLuint texture)
	{
		RENDERCALL(glBindTexture(target, texture), =)
	}

	void RenderSystem::GlTexParameteri(GLenum target, GLenum pname, GLint param)
	{
		RENDERCALL(glTexParameteri(target, pname, param), =)
	}

	void RenderSystem::GlTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels)
	{
		RENDERCALL(glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels), =)
	}

	void RenderSystem::GlDeleteTextures(GLsizei n, const GLuint* textures)
	{
		RENDERCALL(glDeleteTextures(n, textures), =)
	}

	void RenderSystem::GlCreateProgram(unsigned int& id_ret)
	{
		RENDERCALL(id_ret = glCreateProgram(), &)
	}

	void RenderSystem::GlCreateShader(unsigned int& id_ret, GLenum type)
	{
		RENDERCALL(id_ret = glCreateShader(type), &, type)
	}

	void RenderSystem::GlShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length)
	{
		RENDERCALL(glShaderSource(shader, count, string, length), =)
	}

	void RenderSystem::GlCompileShader(GLuint shader)
	{
		RENDERCALL(glCompileShader(shader), =)
	}

	void RenderSystem::GlGetShaderiv(GLuint shader, GLenum pname, GLint* param)
	{
		RENDERCALL(glGetShaderiv(shader, pname, param), =)
	}

	void RenderSystem::GlGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
	{
		RENDERCALL(glGetShaderInfoLog(shader, bufSize, length, infoLog), =)
	}

	void RenderSystem::GlDeleteShader(GLuint shader)
	{
		RENDERCALL(glDeleteShader(shader), =)
	}

	void RenderSystem::GlAttachShader(GLuint program, GLuint shader)
	{
		RENDERCALL(glAttachShader(program, shader), =)
	}

	void RenderSystem::GlLinkProgram(GLuint program)
	{
		RENDERCALL(glLinkProgram(program), =)
	}

	void RenderSystem::GlValidateProgram(GLuint program)
	{
		RENDERCALL(glValidateProgram(program), =)
	}

	void RenderSystem::GlUseProgram(GLuint program)
	{
		RENDERCALL(glUseProgram(program), =)
	}

	void RenderSystem::GlGetUniformLocation(int& location_ret, GLuint program, const GLchar* name)
	{
		RENDERCALL(location_ret = glGetUniformLocation(program, name), =, &location_ret)
	}

	void RenderSystem::GlUniform1i(GLint location, GLint v0)
	{
		RENDERCALL(glUniform1i(location, v0), =)
	}

	void RenderSystem::GlUniform3fv(GLint location, GLsizei count, const GLfloat* value)
	{
		RENDERCALL(glUniform3fv(location, count, value), =)
	}

	void RenderSystem::GlUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		RENDERCALL(glUniform4f(location, v0, v1, v2, v3), =)
	}

	void RenderSystem::GlUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		RENDERCALL(glUniformMatrix4fv(location, count, transpose, value), =)
	}

	void RenderSystem::GlViewport(GLint x, GLint y, GLsizei width, GLsizei height)
	{
		RENDERCALL(glViewport(x, y, width, height), =)
	}

	void RenderSystem::GlEnable(GLenum cap)
	{
		RENDERCALL(glEnable(cap), =)
	}

	void RenderSystem::GlBlendFunc(GLenum sfactor, GLenum dfactor)
	{
		RENDERCALL(glBlendFunc(sfactor, dfactor), =)
	}

	void RenderSystem::GlDepthFunc(GLenum func)
	{
		RENDERCALL(glDepthFunc(func), =)
	}

	void RenderSystem::GlClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
	{
		RENDERCALL(glClearColor(red, green, blue, alpha), =)
	}

	void RenderSystem::GlClearDepth(GLclampd depth)
	{
		RENDERCALL(glClearDepth(depth), =)
	}

	void RenderSystem::GlDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices)
	{
		RENDERCALL(glDrawElements(mode, count, type, indices), =)
	}

	void RenderSystem::GlfwPollEvents()
	{
		RENDERCALL(glfwPollEvents());
	}

	RenderSystem::RenderSystem()
		: render_queue(), varray_list(), global_shader("shader/basic.shader"), global_ibo(), global_buffer(), command_buffer(nullptr)
	{
		varray_list.try_emplace(VertexArrayType::normal);
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<unsigned int>(1);
		varray_list[VertexArrayType::normal].AddBuffer(global_buffer, layout);

		global_shader.Bind();
		global_ibo.Bind();

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));
	}
}