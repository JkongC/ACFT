#ifndef ACFT_VERTEXARRAY_H_
#define ACFT_VERTEXARRAY_H_

namespace ACFT
{
	class VertexBufferLayout;

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;

		void Unbind() const;

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	private:
		unsigned int va_id;
	};
}

#endif

