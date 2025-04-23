module;

#include <glew.h>
#include <glfw3.h>

module Renderer:GLTexture;

import Types;
import Atlas;

namespace GLImplementations
{
	GLTexture::GLTexture(Ref<Atlas> atlas)
	{
		const unsigned char* data = atlas->GetInternalData();
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		m_Width = atlas->GetTotalWidth();
		m_Height = atlas->GetTotalHeight();
		
		glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height);
		glTextureSubImage2D(
			m_ID, 0, 0, 0, m_Width, m_Height,
			GL_RGBA, GL_UNSIGNED_BYTE, data
		);
	}

	GLTexture::GLTexture(Ref<Image> img)
	{
		const unsigned char* data = img->GetInternalData();
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		m_Width = img->GetWidth();
		m_Height = img->GetHeight();

		glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height);
		glTextureSubImage2D(
			m_ID, 0, 0, 0, m_Width, m_Height,
			GL_RGBA, GL_UNSIGNED_BYTE, data
		);
	}

	void GLTexture::Bind(unsigned int id)
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void GLTexture::Unbind(unsigned int = 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

	void GLTexture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
