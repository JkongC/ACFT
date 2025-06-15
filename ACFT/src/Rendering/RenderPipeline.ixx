export module ACFT.RenderPipeline;

import Types;
import Shader;
import Tesselator;
import Camera;

namespace ACFT
{
	export struct Viewport
	{
		int x;
		int y;
		int width;
		int height;
	};

	export enum RenderOp
	{
		clear = 0, draw, none
	};
	
	export struct RenderPass
	{
		Viewport viewport;
		Ref<Camera> camera;
		Ref<Tesselator> vertices;
		RenderOp pre_op;
		RenderOp post_op;
	};

	export struct RenderPipeline
	{
		Ref<Shader> shader;
	};
}