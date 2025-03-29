#ifndef ACFT_CHUNK_H_
#define ACFT_CHUNK_H_

#include "Block/BlockState.h"

namespace ACFT
{
	constexpr int horizontalBlocksInChunks = 16;
	constexpr int verticalBlocksInChunks = 128;
	
	struct ChunkPos
	{
		int x;
		int z;
	};
	
	class Chunk
	{
	public:
		void Serialize();
		void Unserialize(std::filesystem::path& file_path);

	private:
		ChunkPos pos;
		std::array<BlockState, horizontalBlocksInChunks* horizontalBlocksInChunks* verticalBlocksInChunks> block_states;
		entt::entity chunk_entity;
		entt::registry chunk_reg;
	};
}

#endif
