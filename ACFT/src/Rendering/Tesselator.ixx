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
	
	export class Tesselator
	{
	public:
		Tesselator(Primitive mode);
		Tesselator(Tesselator&&) = default;
		~Tesselator() = default;

		void PushVertex(Vertex vtx);
		Vertex& NewVertex();
		const std::vector<Vertex>& GetVertices() const;
		inline Primitive GetMode() const;

	private:
		Primitive m_VertexMode;
		std::vector<Vertex> m_VertexList;
	};
}