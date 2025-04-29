module ACFT;

import Types;
import Log;
import Application;
import Config;
import Atlas;
import Renderer;
import Timer;

namespace ACFT
{
	ACFTItems Initialize()
	{
		Logger::Init();

		auto window = Window::InitWindow();
		auto& renderer = Renderer::InitRenderer(window);

		return { window, renderer };
	}

	void Clean()
	{
		
	}
}
