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
	std::binary_semaphore renderer_context_ready{ 0 };
	std::binary_semaphore app_ready{ 0 };
	std::binary_semaphore renderer_context_cleaned{ 0 };
	std::atomic<bool> running = true;

	static void RenderThreadFunc()
	{
		ThreadFeatures::is_render_thread = true;

		auto window = Window::InitWindow();
		auto& renderer = Renderer::InitRenderer(window);

		renderer_context_ready.release();

		auto& app = Engine::GetApplication();
		app->Init();
		app_ready.release();

		while (!window->ShouldClose())
		{
			app->OnRender();

			if (Config::IsFPSProfilerUsed())
				FPSProfiler::RecordFrame();

			window->PollEvents();
		}

		running = false;

		Engine::GetApplication().reset();

		Renderer::CleanRenderer();

		renderer_context_cleaned.release();
	}
	
	int Engine::Start(int argc, char** argv)
	{
		Logger::Init();

		auto& app = Engine::s_App;

		ThreadManager::CreateThread(Threads::RENDER, true, RenderThreadFunc);
		ThreadManager::DetachThread(Threads::RENDER);

		NormalTimer timer;
		
		if (!renderer_context_ready.try_acquire_for(std::chrono::seconds(5)))
		{
			ACFT_LOG_ERROR("Render thread failed to initialize!");
			exit(-1);
		}

		if (!app_ready.try_acquire_for(std::chrono::seconds(5)))
		{
			ACFT_LOG_ERROR("Application failed to initialize!");
			exit(-1);
		}

		auto window = Renderer::GetRenderer()->GetWindow();
		
		while (running)
		{
			app->OnUpdate(timer.GetElapsed());
			timer.Flush();
		}
		
		Engine::Clean();

		renderer_context_cleaned.acquire();
		
		return 0;
	}

	Ref<Application>& Engine::GetApplication()
	{
		return Engine::s_App;
	}
	
	void Engine::Clean()
	{
		
	}
}
