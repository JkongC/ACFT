export module Renderer;

export import <memory>;
export import Tesselator;

namespace ACFT
{
	export enum RenderAPI
	{
		OpenGL
	};

	export class Renderer
	{
	public:
		static std::shared_ptr<Renderer> GetRenderer(RenderAPI API_type);

		virtual void InitContext() = 0;
		virtual void DrawTesselator(Tesselator tesselator) = 0;
		virtual void SetClearColor(float r, float g, float b, float a) = 0;

		virtual RenderAPI GetRenderAPI() = 0;

		virtual ~Renderer() = default;

	protected:
		Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
	};
}