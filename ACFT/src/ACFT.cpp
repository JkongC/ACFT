#include "stdafx.h"

#include "ACFT.h"
#include "Util/Timer.h"
#include "Log/Logger.h"
#include "Block/Block.h"
#include "Render/Renderer.h"

namespace ACFT
{
	GLFWwindow* Game::gameWindow = nullptr;
	
	ACFT_ERROR_CODE Game::InitGame()
	{
		ACFT::Logger::Init();
		ACFT_LOG_INFO("Launching Game...");
		
		if (InitWindow() == ACFT_ERROR)
			return ACFT_ERROR;

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
		Block block({0, 0, -2});
		
		Camera& camera = Camera::GetInstance();
		camera.SetPos(camera.GetPos() + glm::vec3(0.0f, 2.0f, 0.0f));
		camera.SetPitch(camera.GetPitch() - PI / 10.0f);
		BlockRenderer& block_renderer = BlockRenderer::GetInstance();
		
		NormalTimer timer;
		int ms_to_sleep;
		
		while (!glfwWindowShouldClose(gameWindow)) {
			
			timer.Flush();
			
			block.Tick();
			block_renderer.Draw(block);
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