module;

#include <entt/entity/registry.hpp>

module Vertex;

import Log;

namespace ACFT
{
	
	Vertex::Vertex()
		: m_InternalID(Vertex::g_Manager.create())
	{ }

	Vertex::Vertex(const Vertex& other)
		: m_InternalID(Vertex::g_Manager.create())
	{
		CopyComponents<VertexPos, VertexColorRGB, VertexColorRGBA,
			VertexTexture, VertexUVCoords>(other.m_InternalID, this->m_InternalID);
	}

	Vertex::Vertex(Vertex&& other) noexcept
	{
		this->m_InternalID = other.m_InternalID;
		other.m_InternalID = entt::null;

		this->m_VertexSize = other.m_VertexSize;
		other.m_VertexSize = 0;
	}

	Vertex::~Vertex()
	{
		Vertex::g_Manager.destroy(this->m_InternalID);
	}

	void Vertex::SetPos(float x, float y, float z)
	{
		Vertex::g_Manager.emplace_or_replace<VertexPos>(this->m_InternalID, x, y, z);
		this->m_VertexSize += sizeof(VertexPos);
	}

	void Vertex::SetColor(float r, float g, float b)
	{
		Vertex::g_Manager.emplace_or_replace<VertexColorRGB>(this->m_InternalID, r, g, b);
		this->m_VertexSize += sizeof(VertexColorRGB);
	}

	void Vertex::SetColor(float r, float g, float b, float a)
	{
		Vertex::g_Manager.emplace_or_replace<VertexColorRGBA>(this->m_InternalID, r, g, b, a);
		this->m_VertexSize += sizeof(VertexColorRGBA);
	}

	void Vertex::SetTexture(unsigned int texture)
	{
		Vertex::g_Manager.emplace_or_replace<VertexTexture>(this->m_InternalID, texture);
		this->m_VertexSize += sizeof(VertexTexture);
	}

	void Vertex::SetUVCoords(float u, float v)
	{
		Vertex::g_Manager.emplace_or_replace<VertexUVCoords>(this->m_InternalID, u, v);
		this->m_VertexSize += sizeof(VertexUVCoords);
	}

	VertexBuilder& VertexBuilder::Pos(float x, float y, float z)
	{
		r_Vtx.SetPos(x, y, z);
		return *this;
	}

	VertexBuilder& VertexBuilder::Color(float r, float g, float b)
	{
		r_Vtx.SetColor(r, g, b);
		return *this;
	}

	VertexBuilder& VertexBuilder::Color(float r, float g, float b, float a)
	{
		r_Vtx.SetColor(r, g, b, a);
		return *this;
	}

	VertexBuilder& VertexBuilder::Texture(unsigned int texture)
	{
		r_Vtx.SetTexture(texture);
		return *this;
	}

	VertexBuilder& VertexBuilder::UVCoords(float u, float v)
	{
		r_Vtx.SetUVCoords(u, v);
		return *this;
	}

	VertexBuilder::~VertexBuilder()
	{
		
	}
}