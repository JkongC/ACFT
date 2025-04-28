module;

#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>

#include <Windows.h>
#include <malloc.h>

module Image;

import Log;

namespace ACFT
{
	Image::Image()
		: m_Width(0), m_Height(0), m_Channels(0), m_Data(nullptr)
	{
	}

	Image::Image(const Image& other)
	{
		InitImage(other.GetImagePath());
	}
	
	Image::Image(const std::filesystem::path& path, bool flip_vertically)
	{
		InitImage(path, flip_vertically);
	}

	Image::~Image()
	{
		stbi_image_free(this->m_Data);
	}

	void Image::InitImage(const std::filesystem::path& path, bool flip_vertically)
	{
		if (m_Data)
		{
			ACFT_LOG_WARN("Trying to init an image that is already initialized.");
			return;
		}
	
		stbi_set_flip_vertically_on_load(flip_vertically ? 1 : 0);
		this->m_Data = stbi_load(path.string().c_str(), &this->m_Width, &this->m_Height, &this->m_Channels, 0);

		if (this->m_Data == nullptr)
			ACFT_LOG_WARN("Failed to load Image \"{}\"!", path.string());
	}
}