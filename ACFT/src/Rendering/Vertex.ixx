export module Vertex;

import Base.entt;

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
		Vertex();
		Vertex(const Vertex&) = default;
		Vertex(Vertex&&) = default;

		~Vertex();

		Vertex& Pos(float x, float y, float z);
		Vertex& Color(float r, float g, float b);
		Vertex& Color(float r, float g, float b, float a);
		Vertex& Texture(unsigned int texture);

		/**
		 * Get a specific attribute of the vertex.
		 * 
		 * \return The pointer to the actual attribute. Nullptr if it doesn't exist.
		 */
		template<typename Attribute>
		Attribute* GetAttribute() const;

		size_t GetSize() const { return m_VertexSize; }

	private:
		entt::entity m_InternalID;
		size_t m_VertexSize{ 0 };
		static inline entt::registry g_Manager;
	};
}