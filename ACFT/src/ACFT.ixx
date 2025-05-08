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

namespace ACFT
{
	export class Engine
	{
	public:
		static int Start(int argc, char** argv);

		template<typename T, typename... Args>
		requires std::is_base_of_v<Application, T>
		static void CreateApplication(Args&&... args)
		{
			Engine::s_App = MakeRef<T>(std::forward<Args>(args)...);
		}

	private:
		Engine() = default;
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		Engine& operator=(const Engine&) = delete;

		static void RenderThreadFunc();
		static void EventThreadFunc();

		static void ReleaseRenderer();
		static void ReleaseWindow();
		static void ReleaseApp();

		static void InitWindow();
		static void InitWindowAndDetachContext();
		static void InitRendererContext();
		static void InitApp();

		static void CleanRendererContext();
		static void CleanWindow();

	private:
		static inline Ref<Application> s_App = nullptr;
		static inline Ref<Window> s_Window = nullptr;
	};
}
