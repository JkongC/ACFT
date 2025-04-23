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
		int buffer_size = WideCharToMultiByte(CP_UTF8, 0, other.m_ImagePath.c_str(), -1, nullptr, 0, nullptr, nullptr);
		char* path_buffer = static_cast<char*>(alloca(buffer_size));
		WideCharToMultiByte(CP_UTF8, 0, other.m_ImagePath.c_str(), -1, path_buffer, buffer_size, nullptr, nullptr);
		stbi_set_flip_vertically_on_load(1);
		this->m_Data = stbi_load(path_buffer, &this->m_Width, &this->m_Height, &this->m_Channels, 0);
	}
	
	Image::Image(const std::filesystem::path& path)
	{
		InitImage(path);
	}

	Image::~Image()
	{
		stbi_image_free(this->m_Data);
	}

	void Image::InitImage(const std::filesystem::path& path)
	{
		if (m_Data)
		{
			ACFT_LOG_WARN("Trying to init an image that is already initialized.");
			return;
		}
		
		//int buffer_size = WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, nullptr, 0, nullptr, nullptr);
		//char* path_buffer = static_cast<char*>(alloca(buffer_size));
		//WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, path_buffer, buffer_size, nullptr, nullptr);
		stbi_set_flip_vertically_on_load(1);
		this->m_Data = stbi_load(path.string().c_str(), &this->m_Width, &this->m_Height, &this->m_Channels, 0);
	}
}