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
		static Ref<Renderer> GetRenderer();

		virtual void InitContext() = 0;

		virtual void DrawTesselator(const Tesselator& tesselator) = 0;
		virtual void DrawSprite(const Sprite& sprite) = 0;

		virtual void BeginScene() = 0;
		virtual void EndScene() = 0;

		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void SetPrimitive(Primitive primitive) = 0;

		virtual RenderAPI GetRenderAPI() = 0;

		virtual ~Renderer() = default;

	protected:
		Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
	};
}