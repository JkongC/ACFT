export module ACFT;

export import Types;
export import Log;
export import Application;
export import Config;
export import Atlas;
export import Renderer;
export import Timer;
export import FPSProfiler;

export struct ACFTItems
{
	ACFT::Ref<ACFT::Window> window;
	ACFT::Renderer& renderer;
};

namespace ACFT
{
	export ACFTItems Initialize();
	export void Clean();
}
