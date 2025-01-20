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

		static BlockRenderer& GetInstance()
		{
			static BlockRenderer renderer;
			return renderer;
		}

		/*
		template<unsigned int VertexCount>
		void Draw(VoxelVertices<VertexCount>&& vertices, const glm::ivec3& pos)
		{
			unsigned int current_idx = 0;

			while (current_idx < vertices.count) {
				if (current_idx + 1 + 3 > maxVerteciesPerDraw)
					Flush();

				for (int i = 0; i < 3; i++)
				{
					vb->PushVertex(vertices.vertices[current_idx + i] + pos);
				}
				current_idx += 3;
			}
		}
		*/

		void Draw(const Block& block);

		void Flush();

	private:
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<IndexBuffer> ib;
		std::unique_ptr<VertexArray> va;
		std::unique_ptr<Shader> shader;

		GLsizei sides_should_render = 0;

		BlockRenderer();
		BlockRenderer(BlockRenderer&&) = delete;
		BlockRenderer(const BlockRenderer&) = delete;
	};
}

#endif
