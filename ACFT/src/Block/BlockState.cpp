#include "stdafx.h"

#include "ACFT.h"
#include "BlockState.h"

namespace ACFT
{
	Ref<Texture> BlockState::GetTexture() const
	{
		return *ResourceManager::GetTexture(block_context->block_name);
	}

	void BlockState::Serialize(std::ofstream& out)
	{
	}

	void BlockState::Unserialize(std::ifstream& in)
	{
	}
}