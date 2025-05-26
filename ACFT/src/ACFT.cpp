module;

#include <semaphore>

#include <glew.h>
#include <glfw3.h>

module ACFT;

import Types;
import Log;
import Application;
import Config.Thread;
import Atlas;
import Renderer;
import Timer;
import ACFT.Literals;
import ACFT.RenderingContexts;
import ACFT.Thread;

#include "Rendering/GLImpls/gldbg.h"

#define MTOp(x) if constexpr (fThreads != 0) x

namespace ACFT
{
	constexpr int fThreads = Config::CompileTime::GetFunctionalThreadCount();
	
	std::binary_semaphore windowReady{ 0 };
	std::counting_semaphore<fThreads> appReady{ 0 };
	std::binary_semaphore rendererContextCleaned{ 0 };
	std::binary_semaphore windowCleaned{ 0 };
	std::atomic<bool> running = true;

	template<ptrdiff_t SignalCount>
	void AnnounceStage(std::counting_semaphore<SignalCount>& stage)
	{
		MTOp(stage.release(SignalCount));
	}

	template<ptrdiff_t SignalCount>
	void WaitStage(std::counting_semaphore<SignalCount>& stage)
	{
		MTOp(stage.acquire());
	}

	void Engine::ReleaseRenderer()
	{
		Renderer::Get().reset();
	}

	void Engine::ReleaseWindow()
	{
		Engine::s_Window.reset();
	}

	void Engine::ReleaseApp()
	{
		Engine::s_App.reset();
	}
	
	void Engine::InitWindow()
	{
		auto [width, height] = Config::GetWindowSize();
		Engine::s_Window = Window::InitWindow(width, height, true, Config::GetWindowUserArea());

		AnnounceStage(windowReady);
	}
	
	void Engine::InitWindowAndDetachContext()
	{
		auto [width, height] = Config::GetWindowSize();
		Engine::s_Window = Window::InitWindow(width, height, true, Config::GetWindowUserArea());
		Engine::s_Window->DetachContext();

		AnnounceStage(windowReady);
	}

	void Engine::InitRendererContext()
	{
		WaitStage(windowReady);

		Engine::s_Window->MakeContextCurrent();
		Renderer::Init(Engine::s_Window);
		ShaderLib::Init();
	}

	void Engine::InitApp()
	{
		Engine::s_App->Init();

		AnnounceStage(appReady);
	}

	void Engine::CleanRendererContext()
	{
		Engine::ReleaseApp();
		Engine::ReleaseRenderer();

		AnnounceStage(rendererContextCleaned);
	}

	void Engine::CleanWindow()
	{
		WaitStage(rendererContextCleaned);

		Engine::ReleaseWindow();

		AnnounceStage(windowCleaned);
	}
	
// ------------------------------------------------------------------------
// Engine running logic.

	void Engine::EventThreadFunc()
	{
		InitWindowAndDetachContext();

		WaitStage(appReady);

		auto& window = Engine::s_Window;
		while (!window->ShouldClose())
		{
			window->WaitEvents();
		}

		running = false;

		Engine::CleanWindow();
	}
	
	void Engine::RenderThreadFunc()
	{
		ThreadFeatures::SetRenderThread();
		
		Engine::InitRendererContext();
		Engine::InitApp();

		auto& app = Engine::s_App;
		while (running)
		{
			app->OnRender();

			if (Config::IsFPSProfilerUsed())
				FPSProfiler::RecordFrame();
		}

		Engine::CleanRendererContext();
	}
	
	int Engine::Start(int argc, char** argv)
	{
		using namespace Config::CompileTime;
		
		Logger::Init();
		Config::LockConfig();

		auto& app = Engine::s_App;

		if constexpr (IsEventThreadUsed())
		{
			ThreadManager::CreateThread(Threads::EVENT, true, Engine::EventThreadFunc);
			ThreadManager::DetachThread(Threads::EVENT);

			if constexpr (!IsRenderThreadUsed())
			{
				Engine::InitRendererContext();
			}
		}

		if constexpr (IsRenderThreadUsed())
		{
			if constexpr (!IsEventThreadUsed())
			{
				Engine::InitWindowAndDetachContext();
			}
			
			ThreadManager::CreateThread(Threads::RENDER, true, Engine::RenderThreadFunc);
			ThreadManager::DetachThread(Threads::RENDER);
		}

		if constexpr (!IsEventThreadUsed() && !IsRenderThreadUsed())
		{
			Engine::InitWindow();
			Engine::InitRendererContext();
			Engine::InitApp();
		}

		WaitStage(appReady);

		auto& window = Engine::s_Window;
		NormalTimer timer;

		if (IsEventThreadUsed())
		{
			while (running)
			{
				app->OnUpdate(timer.GetElapsedAndFlush());

				if constexpr (!IsRenderThreadUsed())
				{
					app->OnRender();
				}
			}
		}
		else
		{
			while (!window->ShouldClose())
			{
				app->OnUpdate(timer.GetElapsedAndFlush());

				if constexpr (!IsRenderThreadUsed())
				{
					app->OnRender();
				}

				window->PollEvents();
			}

			running = false;
		}
		
		if constexpr (!IsRenderThreadUsed())
		{
			Engine::CleanRendererContext();
		}

		if constexpr (IsEventThreadUsed())
		{
			WaitStage(windowCleaned);
		}
		else
		{
			Engine::CleanWindow();
		}
		
		return 0;
	}
}
