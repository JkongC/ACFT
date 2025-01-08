#ifndef ACFT_VOXEL_H_
#define ACFT_VOXEL_H_

#include "Texture.h"

namespace ACFT 
{
	class Voxel
	{
	public:
		enum class Side
		{
			front = 0, back, left, right, up, down
		};

		Voxel(const std::string& texture_filepath, float length);
		~Voxel() = default;

		inline float GetLength() const { return length; };

	private:
		float length;
		Texture texture;
	};
}

#endif
