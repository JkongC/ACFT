module;

#include <glew.h>
#include <glfw3.h>

module Renderer:GLFrameBuffer;

import Log;

#include "gldbg.h"

namespace GLImplementations
{
	GLFrameBuffer::GLFrameBuffer(int width, int height)
	{
		GLCall(glGenFramebuffers(1, &m_ID));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ID));

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

		glGenRenderbuffers(1, &m_RenderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);

		if (glCheckFramebufferStatus(m_ID) != GL_FRAMEBUFFER_COMPLETE)
		{
			ACFT_GL_LOG("Failed to create FrameBuffer!");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	GLFrameBuffer::~GLFrameBuffer()
	{
		GLCall(glDeleteTextures(1, &m_TextureID));
		GLCall(glDeleteRenderbuffers(1, &m_RenderBufferID));
		GLCall(glDeleteFramebuffers(1, &m_ID));
	}

	void GLFrameBuffer::Bind()
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ID));
	}

	void GLFrameBuffer::Unbind()
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
}