module ACFT;

import <semaphore>;

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

namespace ACFT
{
	std::binary_semaphore render_thread_ready{ 0 };

	static void RenderThreadFunc()
	{
		ThreadFeatures::is_render_thread = true;

		auto window = Window::InitWindow();
		Renderer::InitRenderer(window);

		render_thread_ready.release();
	}
	
	ACFTItems Start(Ref<Application> app)
	{
		Logger::Init();

		ThreadManager::CreateThread(Threads::RENDER_THREAD, RenderThreadFunc);
		ThreadManager::StartThread(Threads::RENDER_THREAD);

		render_thread_ready.acquire();

		auto& renderer = Renderer::GetRenderer();
		auto window = renderer->GetWindow();
		
		return { window, renderer };
	}
	
	void Clean()
	{
		ThreadManager::JoinThread(Threads::RENDER_THREAD);
	}
}
