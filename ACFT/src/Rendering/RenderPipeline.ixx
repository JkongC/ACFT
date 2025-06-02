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
		RenderOp pre_op;
		RenderOp post_op;
		Viewport viewport;
		Ref<Camera> camera;
		Ref<Tesselator> vertices;
	};

	export struct RenderPipeline
	{
		Ref<Shader> shader;
	};
}