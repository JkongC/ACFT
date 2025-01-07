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
		static GLFWwindow* gameWindow;

	private:
		static ACFT_ERROR_CODE InitWindow();
		static ACFT_ERROR_CODE GameLoop();
	};


}

//Error Code
#define ACFT_NORMAL 0
#define ACFT_ERROR -1

#endif
