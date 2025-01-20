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
		:voxel("resources/acacia_log.png", 1.0f, VoxelVertexType::fullBlock), pos(pos), render_flag(6)
	{
		for (int i = 0; i < 5; i++)
		{
			render_flag[i] = true;
		}

	}

	void Block::Tick()
	{
		UpdateSideVisibility(Camera::GetInstance().GetPos());
	}

	void Block::UpdateSideVisibility(const glm::vec3& camera_pos)
	{
		for (int i = 0; i < 6; i++)
		{
			render_flag[i] = glm::dot(camera_pos - (Block::side_center[i] + pos), side_center[i]) > 0;
		}
	}
}