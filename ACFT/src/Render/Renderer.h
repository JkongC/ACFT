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
		~BlockRenderer() = default;

		static void Render(const Block& block);

	private:
		BlockRenderer();
		BlockRenderer(BlockRenderer&&) = delete;
		BlockRenderer(const BlockRenderer&) = delete;
	};

	class BackgroundRenderer
	{
	public:
		static BackgroundRenderer& GetInstance()
		{
			static BackgroundRenderer renderer;
			return renderer;
		}
	private:
		std::unique_ptr<Shader> shader;
		
		BackgroundRenderer();
		BackgroundRenderer(BackgroundRenderer&&) = delete;
		BackgroundRenderer(const BackgroundRenderer&) = delete;
	};
}

#endif
