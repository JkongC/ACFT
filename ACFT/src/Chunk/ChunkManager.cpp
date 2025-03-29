#include "stdafx.h"

#include "ACFT.h"
#include "ChunkManager.h"
#include "Render/RenderSystem.h"
#include "Serialization/Serializer.h"

import Components;

namespace ACFT
{
	void ChunkManager::RenderChunks()
	{
		for (auto entity : entity_reg.view<MeshComponent>())
		{
			MeshComponent& mesh = entity_reg.get<MeshComponent>(entity);
			RenderSystem::PushVertex(mesh.vertices);
		}
	}

	void ChunkManager::LoadChunk()
	{

	}

	void ChunkManager::UnloadChunk()
	{
	}
}