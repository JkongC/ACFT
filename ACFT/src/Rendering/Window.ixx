export module Window;

import Renderer;
import Types;

namespace ACFT
{
	export class Window
	{
	public:
		static Ref<Window> InitWindow();
		
		inline int GetWidth() const { return m_Width; }
		inline void SetWidth(int width) { m_Width = width; }

		inline int GetHeight() const { return m_Height; }
		inline void SetHeight(int height) { m_Height = height; }

	protected:
		Window() = default;

	protected:
		int m_Width{ 1440 };
		int m_Height{ 900 };
	};
}