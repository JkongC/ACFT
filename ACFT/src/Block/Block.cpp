#include "stdafx.h"

#include "ACFT.h"
#include "Block.h"

namespace ACFT
{
	Block::Block(Voxel& voxel, Vec3i pos)
		:voxel(voxel), pos(pos)
	{

	}

	void Block::OnRender()
	{

	}
}