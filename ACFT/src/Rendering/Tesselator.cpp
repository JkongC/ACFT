module Tesselator;

import <vector>;

namespace ACFT
{
	constexpr const char* PrimitiveToString(Primitive primitive)
	{
		switch (primitive)
		{
		case Primitive::point:
			return "point";
			break;
		case Primitive::line:
			return "line";
			break;
		case Primitive::triangle:
			return "triangle";
			break;
		case Primitive::triangle_fan:
			return "triangle_fan";
			break;
		case Primitive::square:
			return "square";
			break;
		case Primitive::cube:
			return "cube";
			break;
		default:
			return nullptr;
			break;
		}
	}
	
	constexpr unsigned int IndexCountPerPrimitive(Primitive primitive)
	{
		switch (primitive)
		{
		case ACFT::cube:
			return 36;
			break;
		case ACFT::square:
			return 6;
			break;
		case ACFT::triangle_fan:
			return 3;
			break;
		case ACFT::triangle:
			return 3;
			break;
		case ACFT::point:
			return 1;
			break;
		case ACFT::line:
			return 2;
		default:
			return 0;
			break;
		}
	}

	constexpr unsigned int VertexCountPerPrimitive(Primitive primitive)
	{
		switch (primitive)
		{
		case ACFT::cube:
			return 8;
			break;
		case ACFT::square:
			return 4;
			break;
		case ACFT::triangle_fan:
			return 1;
			break;
		case ACFT::triangle:
			return 3;
			break;
		case ACFT::point:
			return 1;
			break;
		case ACFT::line:
			return 2;
		default:
			return 0;
			break;
		}
	}
	
	Tesselator::Tesselator(Primitive mode)
		: m_VertexMode(mode)
	{}

	void Tesselator::PushVertex(Vertex vtx)
	{
		m_VertexList.push_back(vtx);
	}

	Vertex& Tesselator::NewVertex()
	{
		m_VertexList.emplace_back();
		return m_VertexList.back();
	}

	const std::vector<Vertex>& Tesselator::GetVertices() const
	{
		return m_VertexList;
	}

	Primitive Tesselator::GetMode() const
	{
		return m_VertexMode;
	}
}