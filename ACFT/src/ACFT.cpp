module;

#include <semaphore>

#include <glew.h>
#include <glfw3.h>

module ACFT;

import Types;
import Log;
import Application;
import Config;
import Atlas;
import Renderer;
import Timer;
import ACFT.Literals;
import ACFT.RenderingContexts;
import ACFT.Thread;

#include "Rendering/GLImpls/gldbg.h"

namespace ACFT
{
	std::binary_semaphore windowReady{ 0 };
	std::binary_semaphore appReady{ 0 };
	std::binary_semaphore rendererContextCleaned{ 0 };
	std::binary_semaphore windowCleaned{ 0 };
	std::atomic<bool> running = true;
	
	void Engine::EventThreadFunc()
	{
		Engine::s_Window = Window::InitWindow();
		auto& window = Engine::s_Window;

		window->DetachContext();

		windowReady.release();

		while (running)
		{
			window->WaitEvents();
		}

		rendererContextCleaned.acquire();

		Engine::CleanWindow();

		windowCleaned.release();
	}
	
	void Engine::RenderThreadFunc()
	{
		ThreadFeatures::is_render_thread = true;
		
		Engine::s_Window->MakeContextCurrent();
		
		auto& renderer = Renderer::InitRenderer(Engine::s_Window);

		auto& app = Engine::s_App;
		app->Init();

		appReady.release();

		while (running)
		{
			app->OnRender();

			if (Config::IsFPSProfilerUsed())
				FPSProfiler::RecordFrame();
		}

		Engine::CleanApp();

		Engine::CleanRenderer();

		rendererContextCleaned.release();
	}
	
	int Engine::Start(int argc, char** argv)
	{
		using namespace Config::CompileTime;
		
		Logger::Init();

		auto& app = Engine::s_App;

		if constexpr (IsEventThreadUsed())
		{
			ThreadManager::CreateThread(Threads::EVENT, true, Engine::EventThreadFunc);
			ThreadManager::DetachThread(Threads::EVENT);

			windowReady.acquire();

			if constexpr (!IsRenderThreadUsed())
			{
				Engine::s_Window->MakeContextCurrent();
				Renderer::InitRenderer(Engine::s_Window);
				app->Init();
			}
		}

		if constexpr (IsRenderThreadUsed())
		{
			if constexpr (!IsEventThreadUsed())
			{
				Engine::s_Window = Window::InitWindow();
				Engine::s_Window->DetachContext();
				windowReady.release();
			}
			
			ThreadManager::CreateThread(Threads::RENDER, true, Engine::RenderThreadFunc);
			ThreadManager::DetachThread(Threads::RENDER);

			appReady.acquire();
		}

		if constexpr (!IsEventThreadUsed() && !IsRenderThreadUsed())
		{
			Engine::s_Window = Window::InitWindow();
			Renderer::InitRenderer(Engine::s_Window);
			app->Init();
		}

		auto& window = Engine::s_Window;
		NormalTimer timer;
		while (!window->ShouldClose())
		{
			app->OnUpdate(timer.GetElapsedAndFlush());

			if constexpr (!IsRenderThreadUsed())
			{
				app->OnRender();
			}
			
			if constexpr (!IsEventThreadUsed())
			{
				window->PollEvents();
			}
		}

		running = false;

		if constexpr (!IsRenderThreadUsed())
		{
			Engine::CleanApp();
			Engine::CleanRenderer();
			rendererContextCleaned.release();
		}

		if constexpr (IsEventThreadUsed())
		{
			windowCleaned.acquire();
		}
		else
		{
			rendererContextCleaned.acquire();
			Engine::CleanWindow();
		}
		
		return 0;
	}

	void Engine::CleanRenderer()
	{
		Renderer::GetRenderer().reset();
	}

	void Engine::CleanWindow()
	{
		Engine::s_Window.reset();
	}

	void Engine::CleanApp()
	{
		Engine::s_App.reset();
	}
}
