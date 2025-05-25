module;

#include "Platform.h"

export module Window;

import <unordered_map>;
import <functional>;
import Types;
import Layer;

namespace ACFT
{
	export enum class CursorType
	{
		normal = 0, input, crosshair, pointing_hand, h_resize, v_resize, nwse_resize, nesw_resize, all_resize
	};
	
	export class Window
	{
	public:
		static ACFT_API Ref<Window> InitWindow(int width, int height, bool caption_bar = true);
		
		ACFT_API inline int GetWidth() const { return m_Width; }

		ACFT_API inline int GetHeight() const { return m_Height; }

		ACFT_API inline Ref<LayerStack>& GetLayerStack() { return m_Layers; }

		ACFT_API virtual std::pair<int, int> GetPos() = 0;
		ACFT_API virtual void SetPos(int x, int y) = 0;

		ACFT_API inline bool HasCaptionBar() const { return m_HasCaptionBar; }

		ACFT_API virtual bool ShouldClose() = 0;
		ACFT_API virtual void PollEvents() = 0;
		ACFT_API virtual void WaitEvents() = 0;

		ACFT_API virtual void SwapFrameBuffers() = 0;

		ACFT_API virtual void* GetRawWindow() = 0;

		ACFT_API virtual void MakeContextCurrent() = 0;
		ACFT_API virtual void DetachContext() = 0;

		ACFT_API virtual void SetCursor(CursorType cursor) = 0;
		ACFT_API virtual void SetDragAbility(bool allow_drag) = 0;

		ACFT_API virtual void Minimize() = 0;
		ACFT_API virtual void Maximize() = 0;

		ACFT_API virtual void MarkShouldClose() = 0;

		ACFT_API virtual void SetOpacity(float alpha) = 0;

	protected:
		Window(int width, int height, bool caption) : m_Width(width), m_Height(height)
			, m_HasCaptionBar(caption), m_Layers(LayerStack::Create()) {}

	protected:
		int m_Width;
		int m_Height;

		bool m_HasCaptionBar;

		Ref<LayerStack> m_Layers;
	};
}