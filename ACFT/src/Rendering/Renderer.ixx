module;

#include <mutex>

#include "Platform.h"

export module Renderer;

export import <variant>;

export import Types;
export import Tesselator;
export import Sprite;
export import Window;
export import Camera;
export import Shader;
import Event;
import LockfreeQueue;
import Atlas;

namespace ACFT
{
	export enum class RenderAPI
	{
		OpenGL
	};

	export struct SceneContext
	{
		Ref<Camera> camera = nullptr;
	};

	export struct RenderContext
	{
		ModelMatrix model_mat = ModelMatrix(1.0f);
		Ref<Shader> shader = nullptr;
	};

	export class Renderer
	{
	public:
		ACFT_API static Ref<Renderer>& InitRenderer(Ref<Window> window);
		ACFT_API static Ref<Renderer>& GetRenderer();
		ACFT_API static void CleanRenderer();

		ACFT_API virtual void DrawTesselator(const Tesselator& tesselator, RenderContext context = {}) = 0;
		ACFT_API virtual void DrawSprite(const Sprite& sprite, float xpos, float ypos, float width, float height, RenderContext context = {}) = 0;

		ACFT_API virtual void BeginScene(SceneContext context) = 0;
		ACFT_API virtual void EndScene() = 0;

		ACFT_API virtual void SetClearColor(float r, float g, float b, float a) = 0;
		ACFT_API virtual void SetPrimitive(Primitive primitive) = 0;

		ACFT_API virtual void EnableVSync() = 0;
		ACFT_API virtual void DisableVSync() = 0;

		ACFT_API virtual void EnableBlend() = 0;
		ACFT_API virtual void DisableBlend() = 0;

		ACFT_API virtual void SwapWindowFrameBuffers() = 0;
		ACFT_API virtual void Clear() = 0;

		ACFT_API virtual RenderAPI GetRenderAPI() = 0;

		ACFT_API Ref<Window> GetWindow();

		ACFT_API virtual ~Renderer() = default;

	protected:
		Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;

		friend class Sprite;
		friend class Shader;
		
		ACFT_API virtual RenderObjectIdentifier MakeTexture(Ref<Atlas> atlas) = 0;
		ACFT_API virtual RenderObjectIdentifier MakeTexture(Ref<Image> img) = 0;

		ACFT_API virtual RenderObjectIdentifier MakeShader(const std::filesystem::path& shader_path, ShaderLang language = ShaderLang::GLSL, ShaderType type = ShaderType::glsl_mixed) = 0;

	protected:
		Ref<Window> m_Window;
		SceneContext m_SceneContext;
		RenderContext m_RenderContextCache;

		LockfreeQueue<Event, Ref, 500> m_EventQueue;

	private:
		static inline Ref<Renderer> s_Instance = nullptr;
		static inline std::mutex s_Mtx;
	};
}