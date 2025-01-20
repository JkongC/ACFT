#ifndef ACFT_BLOCK_H_
#define ACFT_BLOCK_H_

#include "Render/Voxel.h"
#include "Util/Vec.h"

namespace ACFT
{
	class Block
	{
	public:
		enum class Side
		{
			front = 0, back, left, right, up, down
		};

		Block(glm::ivec3 pos);
		~Block() = default;

		void Tick();

		void UpdateSideVisibility(const glm::vec3& camera_pos);

		inline const Voxel<VoxelVertexType::normalCube>& GetVoxel() const { return voxel; }
		inline float GetLength() const { return voxel.GetLength(); };
		inline const glm::ivec3& GetPos() const { return pos; }
		inline const std::vector<bool> GetRenderFlag() const { return render_flag; }
	private:
		Voxel<VoxelVertexType::normalCube> voxel;
		glm::ivec3 pos;

		std::vector<bool> render_flag;

		static std::array<glm::vec3, 6> side_center;
	};
}

#endif
