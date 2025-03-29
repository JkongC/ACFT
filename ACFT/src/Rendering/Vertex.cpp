module Vertex;

namespace ACFT
{
	struct VertexPos
	{
		float x;
		float y;
		float z;
	};

	struct VertexColorRGB
	{
		int r;
		int g;
		int b;
	};

	struct VertexColorRGBA
	{
		int r;
		int g;
		int b;
		int a;
	};

	struct VertexTexture
	{
		UUID texture;
	};
	
	Vertex::Vertex()
		:m_InternalID(Vertex::g_Manager.create())
	{}

	Vertex::~Vertex()
	{
		Vertex::g_Manager.destroy(this->m_InternalID);
	}

	void Vertex::Pos(float x, float y, float z)
	{
		Vertex::g_Manager.emplace_or_replace<VertexPos>(this->m_InternalID, x, y, z);
	}

	void Vertex::Color(int r, int g, int b)
	{
		Vertex::g_Manager.emplace_or_replace<VertexColorRGB>(this->m_InternalID, r, g, b);
	}

	void Vertex::Color(int r, int g, int b, int a)
	{
		Vertex::g_Manager.emplace_or_replace<VertexColorRGBA>(this->m_InternalID, r, g, b, a);
	}

	void Vertex::Texture(UUID texture)
	{
		Vertex::g_Manager.emplace_or_replace<VertexTexture>(this->m_InternalID, texture);
	}
}