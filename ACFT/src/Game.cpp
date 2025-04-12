module Game;

import Config;
import Renderer;

namespace ACFT
{
	int Game::Run()
	{
		this->m_Window = Window::InitWindow();
		this->m_Renderer = Renderer::GetRenderer();
	}
}