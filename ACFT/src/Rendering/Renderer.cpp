module Renderer;

import Log;
import :OpenGLRenderer;

namespace ACFT
{
	std::shared_ptr<Renderer> Renderer::GetRenderer(RenderAPI API_type)
	{
		switch (API_type)
		{
		case RenderAPI::OpenGL:
			return std::static_pointer_cast<Renderer>(std::make_shared<OpenGLRenderer>());
			break;
		default:
			ACFT_LOG_ERROR("[Renderer] Trying to use an invalid Render API type!");
			return nullptr;
		}
	}
}