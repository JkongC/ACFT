module Config;

import Renderer;

namespace ACFT::Config
{
	static inline RenderAPI RENDER_API = RenderAPI::OpenGL;
	
	void SetRenderAPI(RenderAPI render_API)
	{
		RENDER_API = render_API;
	}

	RenderAPI GetRenderAPI()
	{
		return RENDER_API;
	}
}