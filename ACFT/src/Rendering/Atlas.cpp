module Atlas;

import <utility>;
import Log;
import Image;

namespace ACFT
{
	Atlas::Atlas(int horizontal_textures, int vertical_textures, int ppt)
		: m_MaxHorizontalTextures(horizontal_textures), m_MaxVerticalTextures(vertical_textures)
		, m_PixelPerTexture(ppt)
	{

	}

	Atlas::~Atlas()
	{
		delete this->m_AtlasData;
	}

	void Atlas::InitAtlas(int channels)
	{
		this->m_AtlasData = new unsigned char[
			m_MaxHorizontalTextures * m_PixelPerTexture *
				m_MaxVerticalTextures * m_PixelPerTexture * channels];
		this->m_Channels = channels;
	}
	
	TextureInfo Atlas::AddTexture(Image& img)
	{
		if (img.GetWidth() != m_PixelPerTexture || img.GetHeight() != m_PixelPerTexture)
		{
			ACFT_LOG_ERROR("Inconsistent pixel per texture in atlas is currently not supported!");
			return {};
		}

		if (m_Channels == 0)
		{
			InitAtlas(img.GetChannels());
		}
		else if (m_Channels != img.GetChannels())
		{
			ACFT_LOG_ERROR("Inconsistent channels in atlas is currently not supported!");
			return {};
		}

		if (m_CurrentRow >= m_MaxVerticalTextures)
		{
			ACFT_LOG_ERROR("Trying to add a texture into a full atlas!");
			return {};
		}

		const unsigned char* img_data = img.GetInternalData();
		for (size_t row = 0; row < img.GetHeight(); row++)
		{
			size_t atl_row_offset = m_MaxHorizontalTextures * m_PixelPerTexture * m_Channels * (m_CurrentRow + row);
			size_t atl_col_offset = m_CurrentColumn * m_Channels;
			memcpy(&this->m_AtlasData[atl_row_offset + atl_col_offset], &img.GetInternalData()[static_cast<size_t>(img.GetWidth()) * m_Channels * row], static_cast<size_t>(img.GetWidth()) * m_Channels);
		}
		

		TextureInfo info{};
		info.uv_coords.min_u = static_cast<float>(m_CurrentColumn) / static_cast<float>(m_MaxHorizontalTextures);
		info.uv_coords.min_v = static_cast<float>(m_MaxVerticalTextures - m_CurrentRow - 1) / m_MaxVerticalTextures;
		info.uv_coords.max_u = static_cast<float>(m_CurrentColumn + 1) / static_cast<float>(m_MaxHorizontalTextures);
		info.uv_coords.max_v = static_cast<float>(m_MaxVerticalTextures - m_CurrentRow) / m_MaxVerticalTextures;
		info.width = img.GetWidth();
		info.height = img.GetHeight();

		if (m_CurrentColumn == m_MaxHorizontalTextures - 1)
		{
			m_CurrentRow++;
			m_CurrentColumn = 0;
		}
		else m_CurrentColumn++;

		return info;
	}
}