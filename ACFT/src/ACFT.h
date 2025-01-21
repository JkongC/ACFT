#ifndef ACFT_ACFT_H_
#define ACFT_ACFT_H_

namespace ACFT
{
	using ACFT_ERROR_CODE = int;

	constexpr int WindowWidth = 1280;
	constexpr int WindowHeight = 960;
	constexpr int FramesPerSecond = 120;
	constexpr int MsPerFrame = (int)(1000 / FramesPerSecond);

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

	class Hash
	{
	public:
		virtual std::size_t GetHash() = 0;
		virtual bool operator==(const Hash& other) { return this == &other; }
	};
}

//Error Code
#define ACFT_NORMAL 0
#define ACFT_ERROR -1

//OpenGL Things
void GLClearError();
void GLLogCall();

//OpenGL Callbacks
void MousePosCallback(GLFWwindow* window, double xpos, double ypos);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

//Math Consts
constexpr float PI = glm::pi<float>();

//OpenGL Debug Macros
#define GLCall(x) GLClearError();\
					x;\
					GLLogCall()

#endif
