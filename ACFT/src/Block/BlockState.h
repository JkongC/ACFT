#ifndef ACFT_BLOCKSTATE_H_
#define ACFT_BLOCKSTATE_H_

#include "Block.h"
#include "Blocks.h"

namespace ACFT
{
	class BlockState
	{
	public:
		Ref<Texture> GetTexture() const;

		void Serialize(std::ofstream& out);
		void Unserialize(std::ifstream& in);

	private:
		BlockContextID block_context;
		entt::entity entity_self;
	};
}

#endif
