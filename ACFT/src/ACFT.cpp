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
	
	ACFT_ERROR_CODE Game::InitGame()
	{
		ACFT::Logger::Init();
		ACFT_LOG_INFO("Launching Game...");
		
		if (InitWindow() == ACFT_ERROR)
			return ACFT_ERROR;

		glfwSetCursorPosCallback(gameWindow, MousePosCallback);
		glfwSetMouseButtonCallback(gameWindow, MouseButtonCallback);
		glfwSetKeyCallback(gameWindow, KeyCallback);

		if (GameLoop() == ACFT_ERROR)
			return ACFT_ERROR;

		return ACFT_NORMAL;
	}

	ACFT_ERROR_CODE Game::EndGame()
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

		return ACFT_NORMAL;
	}

	ACFT_ERROR_CODE Game::GameLoop()
	{
		Block block_1({0, 0, -2});
		Block block_2({2, 0, -2});
		
		Camera& camera = Camera::GetInstance();
		camera.SetPos(camera.GetPos() + glm::vec3(0.0f, 2.0f, 0.0f));
		camera.SetPitch(camera.GetPitch() - PI / 10.0f);
		BlockRenderer& block_renderer = BlockRenderer::GetInstance();

		TickManager& tick_manager = TickManager::GetInstance();
		
		NormalTimer timer;
		int ms_to_sleep;
		
		while (!glfwWindowShouldClose(gameWindow)) {
			
			timer.Flush();
			
			tick_manager.TickLogic();
			block_1.Tick();
			block_renderer.Draw(block_1);
			block_2.Tick();
			block_renderer.Draw(block_2);
			block_renderer.Flush();
			
			glfwSwapBuffers(gameWindow);

			glfwPollEvents();

			
			if ((ms_to_sleep = MsPerFrame - timer.GetElapsed()) > 0)
				Sleep(ms_to_sleep);
		}

		return ACFT_NORMAL;
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
	}
#if 0
	__debugbreak();
#endif
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
	bool press = action == GLFW_PRESS;
	ACFT::InputEvent event(press ? ACFT::Event::Type::key_press : ACFT::Event::Type::key_release);
	event.keycode = key;
	event.down = press;
	ACFT::EventManager::GetInstance().Trigger(event);
}