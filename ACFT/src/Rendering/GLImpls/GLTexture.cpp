module;

#include <glew.h>
#include <glfw3.h>

module Renderer:GLTexture;

import Types;
import Atlas;
import Log;

#include "gldbg.h"

namespace GLImplementations
{
	using namespace ACFT;

	GLTexture::GLTexture(Ref<Atlas> atlas)
	{
		const unsigned char* data = atlas->GetInternalData();
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ID));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		m_Width = atlas->GetTotalWidth();
		m_Height = atlas->GetTotalHeight();
		
		GLCall(glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height));
		GLCall(glTextureSubImage2D(
			m_ID, 0, 0, 0, m_Width, m_Height,
			GL_RGBA, GL_UNSIGNED_BYTE, data
		));
	}

	GLTexture::GLTexture(Ref<Image> img)
	{
		const unsigned char* data = img->GetInternalData();
		GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &m_ID));

		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		m_Width = img->GetWidth();
		m_Height = img->GetHeight();

		GLCall(glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height));
		GLCall(glTextureSubImage2D(
			m_ID, 0, 0, 0, m_Width, m_Height,
			GL_RGBA, GL_UNSIGNED_BYTE, data
		));
	}

	GLTexture::GLTexture(GLTexture&& other)
	{
		this->m_Width = other.m_Width;
		this->m_Height = other.m_Height;
		this->m_ID = other.m_ID;
		other.m_ID = 0;
	}

	GLTexture::~GLTexture()
	{
		if (m_ID != 0)
			GLCall(glDeleteTextures(1, &m_ID));
	}

	void GLTexture::Bind(unsigned int id)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, id));
	}

	void GLTexture::Unbind(unsigned int = 0)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void GLTexture::Bind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
	}

	void GLTexture::Unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
}
