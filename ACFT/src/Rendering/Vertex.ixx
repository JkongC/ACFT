module;

#include <entt/entity/registry.hpp>

#include "Platform.h"

export module Vertex;

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
	
	export class Vertex
	{
	public:
		ACFT_API Vertex();
		ACFT_API Vertex(const Vertex&) = default;
		ACFT_API Vertex(Vertex&&) = default;

		ACFT_API ~Vertex();

		ACFT_API Vertex& Pos(float x, float y, float z);
		ACFT_API Vertex& Color(float r, float g, float b);
		ACFT_API Vertex& Color(float r, float g, float b, float a);
		ACFT_API Vertex& Texture(unsigned int texture);

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
		entt::entity m_InternalID;
		size_t m_VertexSize{ 0 };
		static inline entt::registry g_Manager;
	};
}