module ACFT.Screen;

namespace ACFT::UI
{
	bool Screen::ShouldRender() const
	{
		return m_ShouldRender;
	}

	void Screen::Show()
	{
		m_ShouldRender = true;
	}

	void Screen::Hide()
	{
		m_ShouldRender = false;
	}

	const ElementRectLocation& Screen::GetLocation() const
	{
		return m_Location;
	}

	void Screen::SetLocation(const ElementRectLocation& location)
	{
		m_Location = location;
	}
}