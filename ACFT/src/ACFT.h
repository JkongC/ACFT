#ifndef ACFT_ACFT_H_
#define ACFT_ACFT_H_

namespace ACFT
{
	using ACFT_ERROR_CODE = int;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, class... Args>
	inline Ref<T> MakeRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T, class... Args>
	inline Scope<T> MakeScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	constexpr int WindowWidth = 1280;
	constexpr int WindowHeight = 960;
	constexpr int FramesPerSecond = 180;
	constexpr float MsPerFrame = 1000.0f / FramesPerSecond;

	constexpr int maxVerteciesPerDraw = 6 * 300;

	class Game
	{
	public:
		static ACFT_ERROR_CODE Init();
		static ACFT_ERROR_CODE End();

	public:
		inline static GLFWwindow* GetGameWindow() { return gameWindow; };
		inline static bool IsGameRunning() { return running; }
		static bool IsRenderThread();
		inline static void StopGame() { running = false; }

	private:
		static ACFT_ERROR_CODE InitWindow();
		static ACFT_ERROR_CODE GameLoop();
		static void RenderThread();

	private:
		static GLFWwindow* gameWindow;
		static inline std::atomic<bool> running = true;
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

//Key Codes
#define ACFT_KEY_SPACE              32
#define ACFT_KEY_APOSTROPHE         39  /* ' */
#define ACFT_KEY_COMMA              44  /* , */
#define ACFT_KEY_MINUS              45  /* - */
#define ACFT_KEY_PERIOD             46  /* . */
#define ACFT_KEY_SLASH              47  /* / */
#define ACFT_KEY_0                  48
#define ACFT_KEY_1                  49
#define ACFT_KEY_2                  50
#define ACFT_KEY_3                  51
#define ACFT_KEY_4                  52
#define ACFT_KEY_5                  53
#define ACFT_KEY_6                  54
#define ACFT_KEY_7                  55
#define ACFT_KEY_8                  56
#define ACFT_KEY_9                  57
#define ACFT_KEY_SEMICOLON          59  /* ; */
#define ACFT_KEY_EQUAL              61  /* = */
#define ACFT_KEY_A                  65
#define ACFT_KEY_B                  66
#define ACFT_KEY_C                  67
#define ACFT_KEY_D                  68
#define ACFT_KEY_E                  69
#define ACFT_KEY_F                  70
#define ACFT_KEY_G                  71
#define ACFT_KEY_H                  72
#define ACFT_KEY_I                  73
#define ACFT_KEY_J                  74
#define ACFT_KEY_K                  75
#define ACFT_KEY_L                  76
#define ACFT_KEY_M                  77
#define ACFT_KEY_N                  78
#define ACFT_KEY_O                  79
#define ACFT_KEY_P                  80
#define ACFT_KEY_Q                  81
#define ACFT_KEY_R                  82
#define ACFT_KEY_S                  83
#define ACFT_KEY_T                  84
#define ACFT_KEY_U                  85
#define ACFT_KEY_V                  86
#define ACFT_KEY_W                  87
#define ACFT_KEY_X                  88
#define ACFT_KEY_Y                  89
#define ACFT_KEY_Z                  90
#define ACFT_KEY_LEFT_BRACKET       91  /* [ */
#define ACFT_KEY_BACKSLASH          92  /* \ */
#define ACFT_KEY_RIGHT_BRACKET      93  /* ] */
#define ACFT_KEY_GRAVE_ACCENT       96  /* ` */
#define ACFT_KEY_WORLD_1            161 /* non-US #1 */
#define ACFT_KEY_WORLD_2            162 /* non-US #2 */

#define ACFT_KEY_ESCAPE             256
#define ACFT_KEY_ENTER              257
#define ACFT_KEY_TAB                258
#define ACFT_KEY_BACKSPACE          259
#define ACFT_KEY_INSERT             260
#define ACFT_KEY_DELETE             261
#define ACFT_KEY_RIGHT              262
#define ACFT_KEY_LEFT               263
#define ACFT_KEY_DOWN               264
#define ACFT_KEY_UP                 265
#define ACFT_KEY_PAGE_UP            266
#define ACFT_KEY_PAGE_DOWN          267
#define ACFT_KEY_HOME               268
#define ACFT_KEY_END                269
#define ACFT_KEY_CAPS_LOCK          280
#define ACFT_KEY_SCROLL_LOCK        281
#define ACFT_KEY_NUM_LOCK           282
#define ACFT_KEY_PRINT_SCREEN       283
#define ACFT_KEY_PAUSE              284
#define ACFT_KEY_F1                 290
#define ACFT_KEY_F2                 291
#define ACFT_KEY_F3                 292
#define ACFT_KEY_F4                 293
#define ACFT_KEY_F5                 294
#define ACFT_KEY_F6                 295
#define ACFT_KEY_F7                 296
#define ACFT_KEY_F8                 297
#define ACFT_KEY_F9                 298
#define ACFT_KEY_F10                299
#define ACFT_KEY_F11                300
#define ACFT_KEY_F12                301
#define ACFT_KEY_F13                302
#define ACFT_KEY_F14                303
#define ACFT_KEY_F15                304
#define ACFT_KEY_F16                305
#define ACFT_KEY_F17                306
#define ACFT_KEY_F18                307
#define ACFT_KEY_F19                308
#define ACFT_KEY_F20                309
#define ACFT_KEY_F21                310
#define ACFT_KEY_F22                311
#define ACFT_KEY_F23                312
#define ACFT_KEY_F24                313
#define ACFT_KEY_F25                314
#define ACFT_KEY_KP_0               320
#define ACFT_KEY_KP_1               321
#define ACFT_KEY_KP_2               322
#define ACFT_KEY_KP_3               323
#define ACFT_KEY_KP_4               324
#define ACFT_KEY_KP_5               325
#define ACFT_KEY_KP_6               326
#define ACFT_KEY_KP_7               327
#define ACFT_KEY_KP_8               328
#define ACFT_KEY_KP_9               329
#define ACFT_KEY_KP_DECIMAL         330
#define ACFT_KEY_KP_DIVIDE          331
#define ACFT_KEY_KP_MULTIPLY        332
#define ACFT_KEY_KP_SUBTRACT        333
#define ACFT_KEY_KP_ADD             334
#define ACFT_KEY_KP_ENTER           335
#define ACFT_KEY_KP_EQUAL           336
#define ACFT_KEY_LEFT_SHIFT         340
#define ACFT_KEY_LEFT_CONTROL       341
#define ACFT_KEY_LEFT_ALT           342
#define ACFT_KEY_LEFT_SUPER         343
#define ACFT_KEY_RIGHT_SHIFT        344
#define ACFT_KEY_RIGHT_CONTROL      345
#define ACFT_KEY_RIGHT_ALT          346
#define ACFT_KEY_RIGHT_SUPER        347
#define ACFT_KEY_MENU               348

#define ACFT_KEY_LAST               ACFT_KEY_MENU

#endif
