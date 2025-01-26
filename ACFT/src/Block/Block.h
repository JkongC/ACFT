#ifndef ACFT_BLOCK_H_
#define ACFT_BLOCK_H_

#include "Render/Voxel.h"
#include "Util/Vec.h"
#include "Tick/Tick.h"

namespace ACFT
{
	class Block : virtual public LogicTicker
	{
	public:
		enum class Side
		{
			front = 0, back, left, right, up, down
		};

		Block(glm::ivec3 pos);
		~Block() = default;

		void TickLogic(float delta);

		inline const Voxel<VoxelVertexType::normalCube>& GetVoxel() const { return voxel; }
		inline float GetLength() const { return voxel.GetLength(); };
		inline const glm::ivec3& GetPos() const { return pos; }
		inline constexpr int GetSideCount() const { return 6; }
	private:
		Voxel<VoxelVertexType::normalCube> voxel;
		glm::ivec3 pos;

		static std::array<glm::vec3, 6> side_center;
	};
}

#endif
