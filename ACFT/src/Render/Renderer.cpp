#include "stdafx.h"

#include "ACFT.h"
#include "Voxel.h"
#include "Renderer.h"
#include "RenderSystem.h"
#include "Log/Logger.h"
#include "Sky/Sky.h"

namespace ACFT
{
	void BlockRenderer::Render(const Block& block)
	{
		const auto& vertices = block.GetVoxel().GetVertices();

		block.GetVoxel().GetTexture().Bind();

		Ref<VertexPack> vertex_pack = MakeRef<VertexPack>();

		for (int side = 0; side < block.GetSideCount(); side++)
		{
			for (int vertex = 0; vertex < 4; vertex++)
			{
				vertex_pack->Push(vertices.vertices[4 * side + vertex] + block.GetPos());
			}
		}

		RenderSystem::PushVertex(vertex_pack);
	}

	void SkyRenderer::Render()
	{
		RenderSystem::BindSky();
		RenderSystem::FlushFrame();
		RenderSystem::BindGlobal();
	}
}