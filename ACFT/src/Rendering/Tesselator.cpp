module Tesselator;

namespace ACFT
{
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
	
	Tesselator::Tesselator(Primitive mode)
		: m_VertexMode(mode)
	{}

	void Tesselator::AddVertex(Vertex vtx)
	{
		m_VertexList.push_back(vtx);
	}

	const std::vector<Vertex>& Tesselator::GetVertices() const
	{
		return m_VertexList;
	}

	inline Primitive Tesselator::GetMode() const
	{
		return m_VertexMode;
	}
}