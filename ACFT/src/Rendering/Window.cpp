module Window;

import Types;
import Renderer;
import Config;
import :OpenGLWindow;

namespace ACFT
{
	Ref<Window> Window::InitWindow()
	{
		switch (Config::GetRenderAPI())
		{
		case RenderAPI::OpenGL:
			return RefCast<Window>(MakeRef<OpenGLWindow>());
			break;
		default:
			return nullptr;
			break;
		}
	}
}