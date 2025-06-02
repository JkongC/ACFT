module;

#include <glew.h>
#include <glfw3.h>

export module Renderer:GLFrameBuffer;

import Types;

namespace GLImplementations
{
	export class GLFrameBuffer
	{
	public:
		GLFrameBuffer(int width, int height);
		~GLFrameBuffer();

		void Bind();
		void Unbind();

		GLuint GetID() const { return m_ID; }

		GLuint GetRenderBuffer() const { return m_RenderBufferID; }
		GLuint GetTexture() const { return m_TextureID; }

	private:
		GLuint m_ID;
		GLuint m_RenderBufferID;
		GLuint m_TextureID;
	};
}