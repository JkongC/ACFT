#ifndef ACFT_VOXEL_H_
#define ACFT_VOXEL_H_

#include "Texture.h"

class Voxel
{
public:
	Voxel(const std::string& texture_filepath);
	~Voxel() = default;


private:
	float length;
	Texture texture;
};

#endif
