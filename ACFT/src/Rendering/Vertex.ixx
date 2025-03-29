export module Vertex;

import Base.entt;
import UUID;

namespace ACFT
{
	export class Vertex
	{
	public:
		Vertex();
		Vertex(const Vertex&) = default;
		Vertex(Vertex&&) = default;

		~Vertex();

		void Pos(float x, float y, float z);
		void Color(int r, int g, int b);
		void Color(int r, int g, int b, int a);
		void Texture(UUID texture);

	private:
		entt::entity m_InternalID;
		static inline entt::registry g_Manager;
	};
}