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

	export struct UserAreaRect
	{
		float left = 0.0f;
		float right = 0.0f;
		float top = 0.0f;
		float bottom = 0.0f;
		bool use_percentage = false;
	};

	export enum class WindowDrawArea
	{
		whole = 0, user
	};
	
	export class Window
	{
	public:
		static ACFT_API Ref<Window> InitWindow(int width, int height, bool customized_border = false, UserAreaRect user_area = {});
		
		ACFT_API inline int GetWidth() const { return m_Width; }

		ACFT_API inline int GetHeight() const { return m_Height; }

		ACFT_API inline Ref<LayerStack>& GetLayerStack() { return m_Layers; }

		ACFT_API virtual std::pair<int, int> GetPos() = 0;
		ACFT_API virtual void SetPos(int x, int y) = 0;

		ACFT_API virtual bool BorderCustomized() const { return m_CustomizedBorder; }

		ACFT_API virtual bool ShouldClose() = 0;
		ACFT_API virtual void PollEvents() = 0;
		ACFT_API virtual void WaitEvents() = 0;

		ACFT_API virtual void SwapFrameBuffers() = 0;

		ACFT_API virtual void* GetRawWindow() = 0;

		ACFT_API virtual void MakeContextCurrent() = 0;
		ACFT_API virtual void DetachContext() = 0;

		ACFT_API virtual void SetCursor(CursorType cursor) = 0;
		ACFT_API virtual void SetDragAbility(bool allow_drag) = 0;

		// Set user area of the window. Requires the window to be border-customized.
		ACFT_API virtual void SetUserArea(UserAreaRect area);
		ACFT_API virtual const UserAreaRect& GetUserArea() const { return m_UserArea; }

		ACFT_API virtual void Minimize() = 0;
		ACFT_API virtual void Maximize() = 0;

		ACFT_API virtual void MarkShouldClose() = 0;

		ACFT_API virtual void SetOpacity(float alpha) = 0;

	protected:
		Window(int width, int height, bool customized_border, UserAreaRect user_area) : m_Width(width), m_Height(height)
			, m_CustomizedBorder(customized_border), m_UserArea(user_area), m_Layers(LayerStack::Create()) {}

	protected:
		int m_Width;
		int m_Height;

		bool m_CustomizedBorder;
		UserAreaRect m_UserArea;

		Ref<LayerStack> m_Layers;
	};
}