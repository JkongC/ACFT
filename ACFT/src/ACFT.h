#ifndef ACFT_ACFT_H_
#define ACFT_ACFT_H_

namespace ACFT
{
	using ACFT_ERROR_CODE = int;

	constexpr int WindowWidth = 1280;
	constexpr int WindowHeight = 960;

	class Game
	{
	public:
		static ACFT_ERROR_CODE InitGame();
		static ACFT_ERROR_CODE EndGame();

	public:
		inline static GLFWwindow* GetGameWindow() { return gameWindow; };

	private:
		static ACFT_ERROR_CODE InitWindow();
		static ACFT_ERROR_CODE GameLoop();

	private:
		static GLFWwindow* gameWindow;
	};


}

//Error Code
#define ACFT_NORMAL 0
#define ACFT_ERROR -1

//OpenGL Things
void GLClearError();
void GLLogCall();

#define GLCall(x) GLClearError();\
					x;\
					GLLogCall()

#endif
