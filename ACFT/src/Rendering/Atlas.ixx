export module Atlas;

import <utility>;
import <vector>;

export import Image;
export import Texture;

namespace ACFT
{
	export class Atlas
	{
	public:
		Atlas(int horizontal_textures, int vertical_textures);
		~Atlas();

		void InitAtlas(int channels);

		/**
		 * Adds a image to the atlas as a single texture
		 * 
		 * \return The information about the texture in the atlas, UV coords for example.
		 */
		TextureInfo AddTexture(Image& img);
		TextureInfo AddTexture(Image&& img);

		int GetTextureCount();

		const TextureInfo& GetTextureInfo(int idx);

		const unsigned char* GetInternalData();

		int GetTextureWidth();
		int GetTextureHeight();

		int GetTotalWidth();
		int GetTotalHeight();

	private:
		unsigned char* m_AtlasData{ nullptr };
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