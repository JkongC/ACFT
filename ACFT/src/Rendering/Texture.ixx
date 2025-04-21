module;

#include "Platform.h"

export module Texture;

import <utility>;
import <variant>;
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
	export struct Texture
	{
		TextureInfo info;
		RenderObjectIdentifier m_Identifier;
	};
}