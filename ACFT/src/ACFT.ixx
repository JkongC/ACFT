export module ACFT;

export import Base.entt;

export import Types;
export import Log;
export import Input;
export import Application;
export import Layer;
export import Config;
export import Atlas;
export import Renderer;
export import Utils;
export import FPSProfiler;
export import ACFT.Serialization;

export struct ACFTItems
{
	ACFT::Ref<ACFT::Window> window;
	ACFT::Ref<ACFT::Renderer> renderer;
};

namespace ACFT
{
	export ACFTItems Start(Ref<Application> app);
	export void Clean();
}
