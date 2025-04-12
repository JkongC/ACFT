module Renderer;

import Types;
import Log;
import Config;
import :OpenGLRenderer;

namespace ACFT
{
	Ref<Renderer> Renderer::GetRenderer()
	{
		switch (Config::GetRenderAPI())
		{
		case RenderAPI::OpenGL:
			return RefCast<Renderer>(MakeRef<OpenGLRenderer>());
			break;
		default:
			ACFT_LOG_ERROR("[Renderer] Trying to use an invalid Render API type!");
			return nullptr;
		}
	}
}