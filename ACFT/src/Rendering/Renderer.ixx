module;

#include "Platform.h"

export module Renderer;

export import Types;
export import Tesselator;
export import Sprite;

namespace ACFT
{
	export enum class RenderAPI
	{
		OpenGL
	};

	export class Renderer
	{
	public:
		ACFT_API static Ref<Renderer> GetRenderer();

		ACFT_API virtual void InitContext() = 0;

		ACFT_API virtual void DrawTesselator(const Tesselator& tesselator) = 0;
		ACFT_API virtual void DrawSprite(const Sprite& sprite) = 0;

		ACFT_API virtual void BeginScene() = 0;
		ACFT_API virtual void EndScene() = 0;

		ACFT_API virtual void SetClearColor(float r, float g, float b, float a) = 0;
		ACFT_API virtual void SetPrimitive(Primitive primitive) = 0;

		ACFT_API virtual RenderAPI GetRenderAPI() = 0;

		ACFT_API virtual ~Renderer() = default;

	protected:
		Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
	};
}