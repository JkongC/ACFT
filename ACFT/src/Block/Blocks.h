#ifndef ACFT_BLOCKS_H_
#define ACFT_BLOCKS_H_

#include "ResourceManager.h"
import UUID;

namespace ACFT
{
	using BlockContextID = unsigned int;
	
	//Provides basic information of a certain block type.
	struct BlockContext
	{
		std::string block_name = "";
		UUID texture;
		
		static BlockContext&& Create();
		
		BlockContext&& Name(const std::string& block_name) &&;
		BlockContext&& Texture(const std::string& texture_file) &&;

		operator BlockContextID() const;

		BlockContext& operator=(BlockContext&&);
		BlockContext() = default;
		BlockContext(const BlockContext&) = default;
		BlockContext(BlockContext&&);

	private:
		BlockContextID id;
	};

	//Create and manage BlockContextID.
	class BlockRegistry
	{
	public:
		static const BlockContext& Register(BlockContext&& block);

	private:
		inline static std::unordered_map<BlockContextID, BlockContext> registry;
		inline static unsigned int block_count = 0;
	};
	
	//Where to place all block contexts. Intended for global usage.
	namespace Blocks
	{
		inline const BlockContext& ACACIA_LOG =
			BlockRegistry::Register(
				BlockContext::Create().Name("acacia_log").Texture("resources/acacia_log.png")
			);
		
		inline const BlockContext& DIRT = 
			BlockRegistry::Register(
				BlockContext::Create().Name("dirt").Texture("resources/dirt.png")
			);
		
		inline const BlockContext& SAND =
			BlockRegistry::Register(
				BlockContext::Create().Name("sand").Texture("resources/sand.png")
			);
	}
}

#endif
