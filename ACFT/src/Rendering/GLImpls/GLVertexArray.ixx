export module Renderer:GLVertexArray;

export import :GLVertexBuffer;
export import :GLIndexBuffer;

import <memory>;

namespace GLImplementations
{
	class VertexArray
	{
	public:
		VertexArray(const VertexBufferLayout& layout, IndexBuffer&& ibo);
		VertexArray(const VertexBufferLayout& layout, const std::shared_ptr<IndexBuffer>& ibo);
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		inline std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IBO; }

	private:
		void Init(const VertexBufferLayout& layout) const;

	private:
		std::shared_ptr<IndexBuffer> m_IBO;
		unsigned int m_ArrayID;
	};
}