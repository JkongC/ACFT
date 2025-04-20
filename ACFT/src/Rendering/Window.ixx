module;

#include "Platform.h"

export module Window;

import <unordered_map>;
import <functional>;
import Renderer;
import Types;

namespace ACFT
{
	export class Window
	{
	public:
		static ACFT_API Ref<Window> InitWindow();
		
		ACFT_API inline int GetWidth() const { return m_Width; }
		ACFT_API inline void SetWidth(int width) { m_Width = width; }

		ACFT_API inline int GetHeight() const { return m_Height; }
		ACFT_API inline void SetHeight(int height) { m_Height = height; }

		ACFT_API virtual bool ShouldClose() = 0;
		ACFT_API virtual void PollEvents() = 0;

	protected:
		Window() = default;

	protected:
		int m_Width{ 1440 };
		int m_Height{ 900 };
	};
}