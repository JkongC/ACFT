#ifndef ACFT_SKY_H_
#define ACFT_SKY_H_

#include "Render/Vertex.h"
#include "Util/MathHelper.h"

namespace ACFT
{
	constexpr float sqrt2 = Sqrt<float>(2.0);
	constexpr float disc_radius = 512.0f;
	constexpr float disc_height = 16.0f;
	
	inline constexpr VertexPack sky_vertices
	{
		{0.0f, disc_height, 0.0f, 0.0f, 0.0f, 0}, //Center
		{disc_radius, disc_height, 0.0f, 0.0f, 0.0f, 0},
		{disc_radius / sqrt2, disc_height, -disc_radius / sqrt2, 0.0, 0.0, 0},
		{0.0f, disc_height, -disc_radius, 0.0f, 0.0f, 0}, 
		{-disc_radius / sqrt2, disc_height, -disc_radius / sqrt2, 0.0, 0.0, 0},
		{-disc_radius, disc_height, 0.0f, 0.0f, 0.0f, 0},
		{-disc_radius / sqrt2, disc_height, disc_radius / sqrt2, 0.0, 0.0, 0},
		{0.0f, disc_height, disc_radius, 0.0f, 0.0f, 0},
		{disc_radius / sqrt2, disc_height, disc_radius / sqrt2, 0.0, 0.0, 0},
	};
}

#endif
