#ifndef ACFT_RENDERSYSTEM_H_
#define ACFT_RENDERSYSTEM_H_

#include "RenderCommand.h"
#include "RenderQueue.h"
#include "Util/Timer.h"
#include "ResourceManager.h"
#include "Renderer.h"

namespace ACFT
{
	class RenderSystem
	{
	public:
		static void Init();
		static void Shutdown();
		static RenderSystem& GetInstance();

	public:
		//Functions for non-render threads.
		static void StartFrame();
		static void FlushFrame();
		static void EndFrame();

		static void PushVertex(const Ref<VertexPack>& _vertices);
		static int GetCurrentVertexCount();

		static bool PollCommand();

	public:
		//Be extremely cautious when calling those funcions
		//that write things to variables in non-render threads.
		//If ever possible, avoid doing that.
		static void GlGenBuffers(GLsizei count, GLuint* id);
		static void GlBindBuffer(GLenum target, GLuint buffer);
		static void GlBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
		static void GlBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);

		static void GlGenVertexArrays(GLsizei n, GLuint* arrays);
		static void GlBindVertexArray(GLuint array);
		static void GlEnableVertexAttribArray(GLuint index);
		static void GlVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

		static void GlGenTextures(GLsizei n, GLuint* textures);
		static void GlBindTexture(GLenum target, GLuint texture);
		static void GlTexParameteri(GLenum target, GLenum pname, GLint param);
		static void GlTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
		static void GlDeleteTextures(GLsizei n, const GLuint* textures);

		static void GlCreateProgram(unsigned int& id_ret);
		static void GlCreateShader(unsigned int& id_ret, GLenum type);
		static void GlShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
		static void GlCompileShader(GLuint shader);
		static void GlGetShaderiv(GLuint shader, GLenum pname, GLint* param);
		static void GlGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
		static void GlDeleteShader(GLuint shader);
		static void GlAttachShader(GLuint program, GLuint shader);
		static void GlLinkProgram(GLuint program);
		static void GlValidateProgram(GLuint program);
		static void GlUseProgram(GLuint program);

		static void GlGetUniformLocation(int& location_ret, GLuint program, const GLchar* name);
		static void GlUniform1i(GLint location, GLint v0);
		static void GlUniform3fv(GLint location, GLsizei count, const GLfloat* value);
		static void GlUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		static void GlUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

		static void GlViewport(GLint x, GLint y, GLsizei width, GLsizei height);

		static void GlEnable(GLenum cap);
		static void GlBlendFunc(GLenum sfactor, GLenum dfactor);
		static void GlDepthFunc(GLenum func);
		static void GlClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
		static void GlClearDepth(GLclampd depth);

		static void GlDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices);

		static void GlfwPollEvents();

	private:
		RenderQueue render_queue;
		std::vector<RenderCommand>* command_buffer;

		std::unordered_map<VertexArrayType, VertexArray> varray_list;

		Shader global_shader;
		IndexBuffer global_ibo;
		VertexBuffer global_buffer;

		VertexPack local_vertex_buffer;

	private:
		RenderSystem();
		RenderSystem(const RenderSystem&) = delete;
		RenderSystem(RenderSystem&&) = delete;

		static void RecordDrawCall(const RenderCommand& draw_call);
	};
}

#endif
