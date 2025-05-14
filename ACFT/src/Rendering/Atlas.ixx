module;

#include "Platform.h"

export module Atlas;

import <utility>;
import <vector>;

export import Image;
export import Texture;
import Types;

namespace ACFT
{
	export class Atlas
	{
	public:
		ACFT_API Atlas(int horizontal_textures, int vertical_textures);
		ACFT_API ~Atlas();

		ACFT_API void InitAtlas(int channels);

		/**
		 * Adds a image to the atlas as a single texture
		 * 
		 * \return The information about the texture in the atlas, UV coords for example.
		 */
		ACFT_API TextureInfo AddTexture(Image& img);
		ACFT_API TextureInfo AddTexture(Image&& img);

		ACFT_API int GetTextureCount();

		ACFT_API const TextureInfo& GetTextureInfo(int idx);

		ACFT_API const unsigned char* GetInternalData();

		ACFT_API int GetTextureWidth();
		ACFT_API int GetTextureHeight();

		ACFT_API int GetTotalWidth();
		ACFT_API int GetTotalHeight();

	private:
		Scope<unsigned char[]> m_AtlasData;
		int m_WidthPerTexture{ 0 };
		int m_HeightPerTexture{ 0 };
		int m_Channels{ 0 };

		int m_MaxHorizontalTextures;
		int m_MaxVerticalTextures;

		int m_CurrentRow{ 0 };
		int m_CurrentColumn{ 0 };

		int m_CurrentImgCount{ 0 };

		std::vector<TextureInfo> m_TempInfos;
	};
}