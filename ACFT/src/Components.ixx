module;
#include "stdafx.h"
#include "ACFT.h"
#include "Render/Vertex.h"
export module Components;

export import UUID;

export namespace ACFT
{
	struct MeshComponent
	{
		VertexPack mesh;
	};

	struct TextureComponent
	{
		UUID texture;
	};

	struct IntegerPosComponent
	{
		int64_t x;
		int64_t y;
		int64_t z;
	};

	struct FloatPosComponent
	{
		float x;
		float y;
		float z;
	};
}