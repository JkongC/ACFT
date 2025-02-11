#ifndef ACFT_RENDERER_H_
#define ACFT_RENDERER_H_

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include "Vertex.h"
#include "Block/Block.h"

namespace ACFT 
{	
	class BlockRenderer
	{
	public:
		static void Render(const Block& block);

	private:
		BlockRenderer();
		BlockRenderer(BlockRenderer&&) = delete;
		BlockRenderer(const BlockRenderer&) = delete;
	};

	class SkyRenderer
	{
	public:
		static void Render();

	private:
		SkyRenderer();
		SkyRenderer(SkyRenderer&&) = delete;
		SkyRenderer(const SkyRenderer&) = delete;
	};
}

#endif
