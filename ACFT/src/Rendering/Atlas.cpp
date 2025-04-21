module Atlas;

import <utility>;
import Log;
import Image;

namespace ACFT
{
	Atlas::Atlas(int horizontal_textures, int vertical_textures)
		: m_MaxHorizontalTextures(horizontal_textures), m_MaxVerticalTextures(vertical_textures)
	{

	}

	Atlas::~Atlas()
	{
		delete this->m_AtlasData;
	}

	void Atlas::InitAtlas(int channels)
	{
		this->m_AtlasData = new unsigned char[
			m_MaxHorizontalTextures * m_WidthPerTexture *
				m_MaxVerticalTextures * m_HeightPerTexture * channels];
		this->m_Channels = channels;
	}

	TextureInfo Atlas::AddTexture(Image&& img)
	{
		return AddTexture(img);
	}
	
	TextureInfo Atlas::AddTexture(Image& img)
	{
		if (m_WidthPerTexture == 0)
			m_WidthPerTexture = img.GetWidth();
		if (m_HeightPerTexture == 0)
			m_HeightPerTexture = img.GetHeight();

		else if (img.GetWidth() != m_WidthPerTexture || img.GetHeight() != m_HeightPerTexture)
		{
			ACFT_LOG_ERROR("Inconsistent width/height in atlas is currently not supported!");
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
		size_t atl_col_offset = static_cast<size_t>(m_CurrentColumn) * m_Channels;
		size_t atl_per_row_offset = static_cast<size_t>(m_MaxHorizontalTextures) * m_WidthPerTexture * m_Channels;
		for (size_t row = 0; row < img.GetHeight(); row++)
		{
			size_t atl_row_offset = atl_per_row_offset * (m_CurrentRow + row);
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

		m_TempInfos.push_back(info);
		m_CurrentImgCount++;

		return info;
	}

	int Atlas::GetTextureCount()
	{
		return m_CurrentImgCount;
	}

	const TextureInfo& Atlas::GetTextureInfo(int idx)
	{
		static TextureInfo empty{};
		
		if (idx >= m_TempInfos.size())
		{
			ACFT_LOG_WARN("[Atlas] Index exceeded while trying to get TextureInfo. Returning empty info.");
			return empty;
		}

		return m_TempInfos.at(idx);
	}

	const unsigned char* Atlas::GetInternalData()
	{
		return m_AtlasData;
	}

	int Atlas::GetTextureWidth()
	{
		return m_WidthPerTexture;
	}

	int Atlas::GetTextureHeight()
	{
		return m_HeightPerTexture;
	}

	int Atlas::GetTotalWidth()
	{
		return m_WidthPerTexture * m_MaxHorizontalTextures;
	}

	int Atlas::GetTotalHeight()
	{
		return m_HeightPerTexture * m_MaxVerticalTextures;
	}
}