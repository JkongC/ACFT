export module Atlas;

import <utility>;
export import Image;
export import Texture;

namespace ACFT
{
	export class Atlas
	{
	public:
		Atlas(int horizontal_textures, int vertical_textures, int ppt);
		~Atlas();

		void InitAtlas(int channels);

		/**
		 * Adds a image to the atlas as a single texture
		 * 
		 * \return The information about the texture in the atlas, UV coords for example.
		 */
		TextureInfo AddTexture(Image& img);
		TextureInfo AddTexture(Image&& img);

	private:
		unsigned char* m_AtlasData{ nullptr };
		int m_PixelPerTexture{ 0 };
		int m_Channels{ 0 };

		int m_MaxHorizontalTextures;
		int m_MaxVerticalTextures;

		int m_CurrentRow{ 0 };
		int m_CurrentColumn{ 0 };
	};
}