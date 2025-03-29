module Window;

import <memory>;
import Renderer;
import Config;
import :OpenGLWindow;

namespace ACFT
{
	std::shared_ptr<Window> Window::InitWindow()
	{
		switch (Config::GetRenderAPI())
		{
		case RenderAPI::OpenGL:
			return std::static_pointer_cast<Window>(std::make_shared<OpenGLWindow>());
			break;
		default:
			return nullptr;
			break;
		}
	}
}