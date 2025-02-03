#ifndef ACFT_RENDERSYSTEM_H_
#define ACFT_RENDERSYSTEM_H_

#include "RenderCommand.h"
#include "RenderQueue.h"
#include "Util/Timer.h"
#include "ResourceManager.h"

namespace ACFT
{	
	class RenderSystem
	{
	public:
		static void Init();
		static void Shutdown();
		static RenderSystem& GetInstance();

		//Be extremely cautious when calling those funcions
		//that take a reference in non-render threads.
		//If ever possible, avoid doing that.
		void GlGenBuffers(GLsizei count, GLuint* id);
		void GlBindBuffer(GLenum target, GLuint buffer);
		void GlBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
		
		void GlGenVertexArrays(GLsizei n, GLuint* arrays);
		void GlBindVertexArray(GLuint array);
		void GlEnableVertexAttribArray(GLuint index);
		void GlVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
		
		void GlGenTextures(GLsizei n, GLuint* textures);
		void GlBindTexture(GLenum target, GLuint texture);
		void GlTexParameteri(GLenum target, GLenum pname, GLint param);
		void GlTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
		void GlDeleteTextures(GLsizei n, const GLuint* textures);

		void GlCreateProgram(unsigned int& id_ret);
		void GlCreateShader(unsigned int& id_ret, GLenum type);
		void GlShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
		void GlCompileShader(GLuint shader);
		void GlGetShaderiv(GLuint shader, GLenum pname, GLint* param);
		void GlGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
		void GlDeleteShader(GLuint shader);
		void GlAttachShader(GLuint program, GLuint shader);
		void GlLinkProgram(GLuint program);
		void GlValidateProgram(GLuint program);
		void GlUseProgram(GLuint program);

		void GlGetUniformLocation(int& location_ret, GLuint program, const GLchar* name);
		void GlUniform1i(GLint location, GLint v0);
		void GlUniform3fv(GLint location, GLsizei count, const GLfloat* value);
		void GlUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		void GlUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

	private:
		RenderQueue render_queue;
		BlockRenderer& block_renderer;
		std::unordered_map<VertexArrayType, VertexArray> varray_list;

		NormalTimer tick_timer;

	private:
		RenderSystem();
		RenderSystem(const RenderSystem&) = delete;
		RenderSystem(RenderSystem&&) = delete;

		void RecordDrawCall(const Ref<RenderCommand>& draw_call);
	};
}

#endif
