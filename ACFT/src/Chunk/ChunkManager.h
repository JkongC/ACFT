#ifndef ACFT_CHUNKMANAGER_H_
#define ACFT_CHUNKMANAGER_H_

#include "Chunk.h"

namespace ACFT
{
	class ChunkManager
	{
	public:
		static void RenderChunks();

		static void LoadChunk();
		static void UnloadChunk();

	private:
		inline static std::vector<Chunk> chunks;
		inline static entt::registry entity_reg;
	};
}

#endif