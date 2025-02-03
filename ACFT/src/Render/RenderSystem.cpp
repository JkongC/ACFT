#include "stdafx.h"

#include "ACFT.h"
#include "RenderSystem.h"

#define RENDERCALL(function, ...) \
if (!Game::IsRenderThread()) \
	{RecordDrawCall(MakeRef<RenderCommand>([__VA_ARGS__]() -> void {GLCall(function);}));}\
else \
	{GLCall(function);}

namespace ACFT
{
	static RenderSystem* rs_instance = nullptr;

	void RenderSystem::Init()
	{
		if (rs_instance == nullptr)
			rs_instance = new RenderSystem();
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

	void RenderSystem::RecordDrawCall(const Ref<RenderCommand>& draw_call)
	{
		render_queue.PushCommand(draw_call);
	}

	void RenderSystem::GlGenBuffers(GLsizei count, GLuint* id)
	{
		RENDERCALL(glGenBuffers(count, id), =);
	}

	void RenderSystem::GlBindBuffer(GLenum target, GLuint buffer)
	{
		RENDERCALL(glBindBuffer(target, buffer), =);
	}

	void RenderSystem::GlBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
	{
		RENDERCALL(glBufferData(target, size, data, usage), =);
	}

	void RenderSystem::GlGenVertexArrays(GLsizei n, GLuint* arrays)
	{
		RENDERCALL(glGenVertexArrays(n, arrays), =);
	}

	void RenderSystem::GlBindVertexArray(GLuint array)
	{
		RENDERCALL(glBindVertexArray(array), =);
	}

	void RenderSystem::GlEnableVertexAttribArray(GLuint index)
	{
		RENDERCALL(glEnableVertexAttribArray(index), =);
	}

	void RenderSystem::GlVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
	{
		RENDERCALL(glVertexAttribPointer(index, size, type, normalized, stride, pointer), =);
	}

	void RenderSystem::GlGenTextures(GLsizei n, GLuint* textures)
	{
		RENDERCALL(glGenTextures(n, textures), =);
	}

	void RenderSystem::GlBindTexture(GLenum target, GLuint texture)
	{
		RENDERCALL(glBindTexture(target, texture), =);
	}

	void RenderSystem::GlTexParameteri(GLenum target, GLenum pname, GLint param)
	{
		RENDERCALL(glTexParameteri(target, pname, param), =);
	}

	void RenderSystem::GlTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels)
	{
		RENDERCALL(glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels), =);
	}

	void RenderSystem::GlDeleteTextures(GLsizei n, const GLuint* textures)
	{
		RENDERCALL(glDeleteTextures(n, textures), =);
	}

	void RenderSystem::GlCreateProgram(unsigned int& id_ret)
	{
		RENDERCALL(id_ret = glCreateProgram(), &);
	}

	void RenderSystem::GlCreateShader(unsigned int& id_ret, GLenum type)
	{
		RENDERCALL(id_ret = glCreateShader(type), &, type);
	}

	void RenderSystem::GlShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length)
	{
		RENDERCALL(glShaderSource(shader, count, string, length), =);
	}

	void RenderSystem::GlCompileShader(GLuint shader)
	{
		RENDERCALL(glCompileShader(shader), =);
	}

	void RenderSystem::GlGetShaderiv(GLuint shader, GLenum pname, GLint* param)
	{
		RENDERCALL(glGetShaderiv(shader, pname, param), =);
	}

	void RenderSystem::GlGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
	{
		RENDERCALL(glGetShaderInfoLog(shader, bufSize, length, infoLog), =);
	}

	void RenderSystem::GlDeleteShader(GLuint shader)
	{
		RENDERCALL(glDeleteShader(shader), =);
	}

	void RenderSystem::GlAttachShader(GLuint program, GLuint shader)
	{
		RENDERCALL(glAttachShader(program, shader), =);
	}

	void RenderSystem::GlLinkProgram(GLuint program)
	{
		RENDERCALL(glLinkProgram(program), =);
	}

	void RenderSystem::GlValidateProgram(GLuint program)
	{
		RENDERCALL(glValidateProgram(program), =);
	}

	void RenderSystem::GlUseProgram(GLuint program)
	{
		RENDERCALL(glUseProgram(program), =);
	}

	void RenderSystem::GlGetUniformLocation(int& location_ret, GLuint program, const GLchar* name)
	{
		RENDERCALL(location_ret = glGetUniformLocation(program, name), =, &location_ret);
	}

	void RenderSystem::GlUniform1i(GLint location, GLint v0)
	{
		RENDERCALL(glUniform1i(location, v0), =);
	}

	void RenderSystem::GlUniform3fv(GLint location, GLsizei count, const GLfloat* value)
	{
		RENDERCALL(glUniform3fv(location, count, value), =);
	}

	void RenderSystem::GlUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		RENDERCALL(glUniform4f(location, v0, v1, v2, v3), =);
	}

	void RenderSystem::GlUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		RENDERCALL(glUniformMatrix4fv(location, count, transpose, value), =);
	}

	RenderSystem::RenderSystem()
		: render_queue(), block_renderer(BlockRenderer::GetInstance()), tick_timer(), varray_list()
	{

	}
}