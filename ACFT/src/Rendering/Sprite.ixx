module;

#include "Platform.h"

export module Sprite;

import <vector>;

import Types;
import Atlas;

namespace ACFT
{
	export class Sprite
	{
	public:
		ACFT_API Sprite() = default;

		ACFT_API void UseAtlas(Ref<Atlas> atlas);
		ACFT_API void UseAtlas(Scope<Atlas> atlas);

		ACFT_API void SetInterval(float interval_ms);

		ACFT_API void AccumulateTime(float time_step);

		ACFT_API const Texture& GetCurrentImage() const;

		ACFT_API int GetImageCount() const { return m_ImgCount; }

	private:
		int m_ImgCount = 0;
		mutable int m_CurrentIdx = 0;
		float m_IntervalMs = 0;
		float m_Timer = 0;
		std::vector<Texture> m_Textures;
	};
}