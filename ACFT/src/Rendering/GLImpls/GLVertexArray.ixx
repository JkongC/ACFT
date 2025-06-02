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
		VertexArray(const VertexBufferLayout& layout, const Ref<VertexBuffer>& vbo, const Ref<IndexBuffer>& ibo);
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		inline Ref<IndexBuffer> GetIndexBuffer() const { return m_IBO; }

	private:
		void Init(const VertexBuffer& vbo, const VertexBufferLayout& layout) const;

	private:
		Ref<IndexBuffer> m_IBO;
		unsigned int m_ArrayID;
	};
}