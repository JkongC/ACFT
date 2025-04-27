module;

#include <entt/entity/registry.hpp>

#include "Platform.h"

export module Vertex;

import Log;

namespace ACFT
{
	export struct VertexPos
	{
		float x;
		float y;
		float z;
	};

	export struct VertexColorRGB
	{
		float r;
		float g;
		float b;
	};

	export struct VertexColorRGBA
	{
		float r;
		float g;
		float b;
		float a;
	};

	export struct VertexTexture
	{
		unsigned int texture;
	};

	export struct VertexUVCoords
	{
		float u;
		float v;
	};
	
	export class Vertex
	{
	public:
		ACFT_API Vertex();
		ACFT_API Vertex(const Vertex&);
		ACFT_API Vertex(Vertex&&) noexcept;

		ACFT_API ~Vertex();

		ACFT_API void SetPos(float x, float y, float z);
		ACFT_API void SetColor(float r, float g, float b);
		ACFT_API void SetColor(float r, float g, float b, float a);
		ACFT_API void SetTexture(unsigned int texture);
		ACFT_API void SetUVCoords(float u, float v);

		/**
		 * Get a specific attribute of the vertex.
		 * 
		 * \return The pointer to the actual attribute. Nullptr if it doesn't exist.
		 */
		template<typename Attribute>
		Attribute* GetAttribute() const
		{
			return Vertex::g_Manager.try_get<Attribute>(this->m_InternalID);
		}

		ACFT_API size_t GetSize() const { return m_VertexSize; }

	private:
		template<typename... Components>
		static void CopyComponents(entt::entity src, entt::entity dst)
		{
			([&] {
				if (auto* cpn = Vertex::g_Manager.try_get<Components>(src))
				{
					Vertex::g_Manager.emplace_or_replace<Components>(dst, *cpn);
				}
			}(), ...);
		}

	private:
		entt::entity m_InternalID;
		size_t m_VertexSize{ 0 };
		static inline entt::registry g_Manager;
	};

	export class VertexBuilder
	{
	public:
		ACFT_API VertexBuilder& Pos(float x, float y, float z);
		ACFT_API VertexBuilder& Color(float r, float g, float b);
		ACFT_API VertexBuilder& Color(float r, float g, float b, float a);
		ACFT_API VertexBuilder& Texture(unsigned int texture);
		ACFT_API VertexBuilder& UVCoords(float u, float v);

		~VertexBuilder();

	private:
		friend class Vertex;
		friend class Tesselator;
		VertexBuilder(Vertex& vtx) : r_Vtx(vtx) {}

	private:
		Vertex& r_Vtx;
	};
}