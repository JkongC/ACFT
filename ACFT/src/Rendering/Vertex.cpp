module;

#include <entt/entity/registry.hpp>

module Vertex;

namespace ACFT
{
	Vertex::Vertex()
		:m_InternalID(Vertex::g_Manager.create())
	{}

	Vertex::~Vertex()
	{
		Vertex::g_Manager.destroy(this->m_InternalID);
	}

	Vertex& Vertex::Pos(float x, float y, float z)
	{
		Vertex::g_Manager.emplace_or_replace<VertexPos>(this->m_InternalID, x, y, z);
		this->m_VertexSize += sizeof(VertexPos);
		return *this;
	}

	Vertex& Vertex::Color(float r, float g, float b)
	{
		Vertex::g_Manager.emplace_or_replace<VertexColorRGB>(this->m_InternalID, r, g, b);
		this->m_VertexSize += sizeof(VertexColorRGB);
		return *this;
	}

	Vertex& Vertex::Color(float r, float g, float b, float a)
	{
		Vertex::g_Manager.emplace_or_replace<VertexColorRGBA>(this->m_InternalID, r, g, b, a);
		this->m_VertexSize += sizeof(VertexColorRGBA);
		return *this;
	}

	Vertex& Vertex::Texture(unsigned int texture)
	{
		Vertex::g_Manager.emplace_or_replace<VertexTexture>(this->m_InternalID, texture);
		this->m_VertexSize += sizeof(VertexTexture);
		return *this;
	}

	Vertex& Vertex::UVCoords(float u, float v)
	{
		Vertex::g_Manager.emplace_or_replace<VertexUVCoords>(this->m_InternalID, u, v);
		this->m_VertexSize += sizeof(VertexUVCoords);
		return *this;
	}
}