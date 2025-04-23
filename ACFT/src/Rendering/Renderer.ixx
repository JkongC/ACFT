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
		ModelMatrix m_mat = ModelMatrix(1.0f);
	};

	export class Renderer
	{
	public:
		ACFT_API static Renderer& InitRenderer(Ref<Window> window);
		ACFT_API static Renderer& GetRenderer();
		ACFT_API static void ShutdownRenderer();

		ACFT_API virtual void InitContext() = 0;

		ACFT_API virtual void DrawTesselator(const Tesselator& tesselator, RenderContext context = {}) = 0;
		ACFT_API virtual void DrawSprite(const Sprite& sprite, float xpos, float ypos, float width, float height, RenderContext context = {}) = 0;

		ACFT_API virtual void BeginScene(SceneContext context) = 0;
		ACFT_API virtual void EndScene() = 0;

		ACFT_API virtual void SetClearColor(float r, float g, float b, float a) = 0;
		ACFT_API virtual void SetPrimitive(Primitive primitive) = 0;

		ACFT_API virtual RenderObjectIdentifier MakeTexture(Ref<Atlas> atlas) = 0;
		ACFT_API virtual RenderObjectIdentifier MakeTexture(Ref<Image> img) = 0;

		ACFT_API virtual RenderAPI GetRenderAPI() = 0;

		ACFT_API virtual ~Renderer() = default;

	protected:
		Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;

	protected:
		Ref<Window> m_Window;
		SceneContext m_SceneContext;

	private:
		static inline Renderer* s_Instance = nullptr;
		static inline std::mutex s_Mtx;
	};
}