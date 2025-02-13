#ifndef ACFT_VERTEXARRAY_H_
#define ACFT_VERTEXARRAY_H_

namespace ACFT
{
	class VertexBufferLayout;
	class RenderSystem;

	enum VertexArrayType
	{
		normal = 0, sky
	};

	class VertexArray
	{
	public:
		VertexArray();
		
		void Bind() const;

		void Unbind() const;

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, const IndexBuffer& ibo) const;

	private:
		unsigned int va_id;
	};
}

#endif

