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

		Block(Voxel& voxel, Vec3i pos);
		~Block() = default;

		void OnRender();

		inline Voxel& GetVoxel() { return voxel; }
		inline float GetLength() const { return voxel.GetLength(); };
	private:
		Voxel& voxel;
		Vec3i pos;
	};
}

#endif
