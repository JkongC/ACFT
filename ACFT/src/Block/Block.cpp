#include "stdafx.h"

#include "ACFT.h"
#include "Block.h"
#include "Util/Direction.h"
#include "Render/Camera.h"

namespace ACFT
{
	std::array<glm::vec3, 6> Block::side_center = {
		glm::vec3(Direction::front * 0.5f),
		glm::vec3(Direction::back * 0.5f),
		glm::vec3(Direction::left * 0.5f),
		glm::vec3(Direction::right * 0.5f),
		glm::vec3(Direction::up * 0.5f),
		glm::vec3(Direction::down * 0.5f),
	};
	
	Block::Block(glm::ivec3 pos)
		:voxel("resources/acacia_log.png", 1.0f, VoxelVertexType::fullBlock), pos(pos)
	{
	}

	void Block::TickLogic(float delta)
	{
		
	}
}