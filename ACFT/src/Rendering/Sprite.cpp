module Sprite;

import <vector>;

import Types;
import Image;
import Renderer;
import Log;

namespace ACFT
{
	void Sprite::UseAtlas(Ref<Atlas> atlas)
	{
		RenderObjectIdentifier id = Renderer::GetRenderer()->MakeTexture(atlas);

		for (int i = 0; i < atlas->GetTextureCount(); i++)
		{
			m_Textures.push_back({ atlas->GetTextureInfo(i), id });
		}

		m_ImgCount = atlas->GetTextureCount();
	}

	void Sprite::UseAtlas(Scope<Atlas> atlas)
	{
		RenderObjectIdentifier id = Renderer::GetRenderer()->MakeTexture(std::move(atlas));

		for (int i = 0; i < atlas->GetTextureCount(); i++)
		{
			m_Textures.push_back({ atlas->GetTextureInfo(i), id });
		}

		m_ImgCount = atlas->GetTextureCount();
	}

	void Sprite::SetInterval(float interval_ms)
	{
		m_IntervalMs = interval_ms;
	}

	void Sprite::AccumulateTime(float time_step)
	{
		m_Timer += time_step;
		while (m_Timer >= m_IntervalMs)
		{
			m_Timer -= m_IntervalMs;
			m_CurrentIdx++;
		}

		if (m_CurrentIdx >= m_ImgCount)
			m_CurrentIdx = 0;
	}

	const Texture& Sprite::GetCurrentImage() const
	{
		if (m_CurrentIdx >= m_ImgCount)
			m_CurrentIdx = 0;

		return m_Textures.at(m_CurrentIdx);
	}
}

