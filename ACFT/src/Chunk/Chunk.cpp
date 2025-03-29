#include "stdafx.h"

#include "ACFT.h"
#include "Chunk.h"
#include "Serialization/Serializer.h"

namespace ACFT
{
	namespace fs = std::filesystem;
	void Chunk::Serialize()
	{
		fs::path output = fs::current_path() / "chunks" / std::format("{}_{}.ck", pos.x, pos.z);
		std::ofstream out(output, std::ios::binary);

		WritePlain<ChunkPos>(pos, out);
		for (size_t i = 0; i < block_states.size(); i++)
		{
			block_states.at(i).Serialize(out);
		}
	}

	void Chunk::Unserialize(fs::path& file_path)
	{
		std::ifstream in(file_path, std::ios::binary);

		ReadPlain<ChunkPos>(pos, in);
		for (size_t i = 0; i < block_states.size(); i++)
		{
			block_states.at(i).Unserialize(in);
		}
	}
}