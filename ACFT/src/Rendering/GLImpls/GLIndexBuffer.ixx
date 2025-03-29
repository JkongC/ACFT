export module Renderer:GLIndexBuffer;

import Tesselator;

namespace GLImplementations
{
	using namespace ACFT;
	class IndexBuffer
	{
	public:
		IndexBuffer();
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&&) = default;
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		template<Primitive mode>
		void GenerateByMode(unsigned int max_primitive_count);

		template<Primitive mode>
		void GenerateByMode();

		inline size_t GetCount() const { return m_Count; }
		inline size_t GetSize() const { return m_Size; }

	private:
		unsigned int m_BufferID;
		size_t m_Count{0};
		size_t m_Size{0};
		bool m_Dynamic{false};
	};
}