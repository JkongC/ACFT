#ifndef ACFT_VERTEXARRAY_H_
#define ACFT_VERTEXARRAY_H_

namespace ACFT
{
	class VertexBufferLayout;

	enum VertexArrayType
	{
		block = 0
	};

	class VertexArray
	{
	public:
		VertexArray();

		void Bind() const;

		void Unbind() const;

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;

	private:
		unsigned int va_id;
	};
}

#endif

