#include "stdafx.h"

#include "ACFT.h"
#include "Util/Timer.h"
#include "Log/Logger.h"
#include "Block/Block.h"
#include "Render/Renderer.h"
#include "Event/EventManager.h"
#include "Tick/Tick.h"

namespace ACFT
{
	GLFWwindow* Game::gameWindow = nullptr;
	static thread_local bool is_render_thread = false;

	void* block_1;
	void* block_2;
	void* block_3;

	static bool windowCreated = false;
	static std::condition_variable renderReady;
	static std::mutex mtx;
	
	ACFT_ERROR_CODE Game::Init()
	{
		ACFT::Logger::Init();
		ACFT_LOG_INFO("Launching Game...");

		ScopedTimer<float, second> game_time([](float elapsed) -> void {ACFT_LOG_TRACE("Game ran for {} s.", elapsed);});

		std::thread render_thread(RenderThread);

		std::unique_lock<std::mutex> lock(mtx);
		
		renderReady.wait(lock, [&]() -> bool {return windowCreated;});
		
		if (GameLoop() == ACFT_ERROR)
			return ACFT_ERROR;

		render_thread.join();
		return ACFT_NORMAL;
	}

	ACFT_ERROR_CODE Game::End()
	{
		ACFT_LOG_INFO("Closing Game...");
		
		glfwTerminate();
		return ACFT_NORMAL;
	}

	ACFT_ERROR_CODE Game::InitWindow()
	{
		if (!glfwInit())
			return ACFT_ERROR;

		gameWindow = glfwCreateWindow(WindowWidth, WindowHeight, "AnotherCraft", nullptr, nullptr);
		if (!gameWindow)
		{
			glfwTerminate();
			return ACFT_ERROR;
		}

		glfwMakeContextCurrent(gameWindow);

		if (glewInit() != GLEW_OK)
			return ACFT_ERROR;

		glfwSetCursorPosCallback(gameWindow, MousePosCallback);
		glfwSetMouseButtonCallback(gameWindow, MouseButtonCallback);
		glfwSetKeyCallback(gameWindow, KeyCallback);

		return ACFT_NORMAL;
	}

	bool Game::IsRenderThread()
	{
		return is_render_thread;
	}

	ACFT_ERROR_CODE Game::GameLoop()
	{
		Camera& camera = Camera::GetInstance();
		camera.SetPos(camera.GetPos() + glm::vec3(0.0f, 2.0f, 0.0f));
		camera.SetPitch(camera.GetPitch() - PI / 10.0f);

		TickManager& tick_manager = TickManager::GetInstance();
		
		NormalTimer timer;
		while (!glfwWindowShouldClose(gameWindow)) {
			
			timer.Flush();
			
			tick_manager.TickLogic();
			//block_renderer.Draw(*(Block*)block_1);
			//block_renderer.Draw(*(Block*)block_2);
			//block_renderer.Flush();
			
			//glfwSwapBuffers(gameWindow);

			if (!running)
				return ACFT_ERROR;
		}

		running = false;
		return ACFT_NORMAL;
	}

	void Game::RenderThread()
	{
		is_render_thread = true;
		
		if (InitWindow() == ACFT_ERROR)
		{
			ACFT_LOG_ERROR("Failed to create the window!");
			running = false;
			return;
		}

		block_1 = new Block({ 0, 0, -2 });
		block_2 = new Block({ 2, 0, -2 });

		{
			std::lock_guard<std::mutex> lock(mtx);
			windowCreated = true;
		}
		renderReady.notify_all();
		
		BlockRenderer& block_renderer = BlockRenderer::GetInstance();
		
		NormalTimer timer;
		while (running)
		{
			float elapsed = timer.GetElapsed();
			if (elapsed < MsPerFrame)
				continue;
			else
				timer.Decline(MsPerFrame);

			block_renderer.Draw(*(Block*)block_1);
			block_renderer.Draw(*(Block*)block_2);
			block_renderer.Flush();

			glfwSwapBuffers(gameWindow);

			glfwPollEvents();
			
		}

		delete (Block*)block_1;
		delete (Block*)block_2;
		delete (Block*)block_3;
	}

}

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

void GLLogCall() {
	GLenum err;
	while ((err = glGetError())) {
		ACFT_GL_LOG("Error occured: {}", err);

#if 0
		__debugbreak();
#endif
	}
}

void MousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
	ACFT::InputEvent event(ACFT::Event::Type::mouse_move);
	event.xpos = xpos;
	event.ypos = ypos;
	ACFT::EventManager::GetInstance().Trigger(event);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	bool press = action == GLFW_PRESS;
	ACFT::InputEvent event(press ? ACFT::Event::Type::mouse_keydown : ACFT::Event::Type::mouse_keyup);
	event.keycode = button;
	event.down = press;
	ACFT::EventManager::GetInstance().Trigger(event);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ACFT::Event::Type type;
	switch (action)
	{
	case GLFW_PRESS:
		type = ACFT::Event::Type::key_press;
		break;
	case GLFW_RELEASE:
		type = ACFT::Event::Type::key_release;
		break;
	case GLFW_REPEAT:
		type = ACFT::Event::Type::key_repeat;
		break;
	default:
		break;
	}
	ACFT::InputEvent event(type);
	event.keycode = key;
	event.down = type == ACFT::Event::Type::key_press || type == ACFT::Event::Type::key_repeat;
	ACFT::EventManager::GetInstance().Trigger(event);
}