module;

#include "Platform.h"

export module Tesselator;

import <vector>;
import <memory>;
import Vertex;

namespace ACFT
{
	export enum Primitive
	{
		point, line, triangle, triangle_fan, square, cube
	};

	export constexpr const char* PrimitiveToString(Primitive primitive);
	export constexpr unsigned int IndexCountPerPrimitive(Primitive primitive);
	export constexpr unsigned int VertexCountPerPrimitive(Primitive primitive);
	
	//A tool for building a specific shape for render use using vertices.
	export class Tesselator
	{
	public:
		ACFT_API Tesselator(Primitive mode);
		ACFT_API Tesselator(Tesselator&&) = default;
		ACFT_API ~Tesselator() = default;

		ACFT_API void PushVertex(Vertex vtx);
		ACFT_API Vertex& NewVertex();
		ACFT_API const std::vector<Vertex>& GetVertices() const;
		ACFT_API Primitive GetMode() const;

	private:
		Primitive m_VertexMode;
		std::vector<Vertex> m_VertexList;
	};
}