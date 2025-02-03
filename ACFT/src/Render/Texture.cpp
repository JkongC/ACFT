#include "stdafx.h"

#include "ACFT.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>

namespace ACFT
{
	Texture::Texture(const std::string& file)
		:file_path(file), local_buffer(nullptr), width(0), height(0), BPP(0), texture_id(0)
	{
		stbi_set_flip_vertically_on_load(1);
		local_buffer = stbi_load(file.c_str(), &width, &height, &BPP, 4);

		GLCall(glGenTextures(1, &texture_id));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture_id));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer));
	}

	Texture::~Texture()
	{
		if (local_buffer)
			stbi_image_free(local_buffer);

		GLCall(glDeleteTextures(1, &texture_id));
	}

	void Texture::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
	}
}