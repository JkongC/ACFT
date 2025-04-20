module;

#include "Platform.h"

export module Texture;

import <utility>;
import Types;

namespace ACFT
{
	export struct TextureUV
	{
		float min_u;
		float min_v;
		float max_u;
		float max_v;
	};
	
	export struct TextureInfo
	{
		TextureUV uv_coords;
		int width;
		int height;
	};
	
	class Atlas;
	export class Texture
	{
	public:
		ACFT_API Texture(const TextureInfo& info, Ref<Atlas> atlas)
			: m_UV(info.uv_coords), m_Width(info.width), m_Height(info.height), m_Atlas(atlas)
		{ }

	private:
		TextureUV m_UV;
		int m_Width;
		int m_Height;
		Ref<Atlas> m_Atlas;
	};
}