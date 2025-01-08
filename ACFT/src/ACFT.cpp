#include "stdafx.h"

#include "ACFT.h"
#include "Log/Logger.h"

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
		while (!glfwWindowShouldClose(gameWindow)) {

			glfwSwapBuffers(gameWindow);

			glfwPollEvents();
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
}