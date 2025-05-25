module ACFT.Screen;

namespace ACFT
{
	bool Screen::ShouldRender() const
	{
		return m_Attr.should_render;
	}

	void Screen::Show()
	{
		m_Attr.should_render = true;
	}

	void Screen::Hide()
	{
		m_Attr.should_render = false;
	}

	const ScreenLocation& Screen::GetLocation() const
	{
		return m_Attr.location;
	}

	void Screen::SetLocation(ScreenLocation location)
	{
		m_Attr.location = location;
	}
}