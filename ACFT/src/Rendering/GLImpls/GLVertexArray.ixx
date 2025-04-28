export module Renderer:GLVertexArray;

export import :GLVertexBuffer;
export import :GLIndexBuffer;

import Types;

namespace GLImplementations
{
	class VertexArray
	{
	public:
		VertexArray(const VertexBufferLayout& layout, const VertexBuffer& vbo, IndexBuffer&& ibo);
		VertexArray(const VertexBufferLayout& layout, const ACFT::Ref<VertexBuffer>& vbo, const ACFT::Ref<IndexBuffer>& ibo);
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		inline std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IBO; }

	private:
		void Init(const VertexBuffer& vbo, const VertexBufferLayout& layout) const;

	private:
		ACFT::Ref<IndexBuffer> m_IBO;
		unsigned int m_ArrayID;
	};
}