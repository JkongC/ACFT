#ifndef ACFT_VERTEXBUFFER_H_
#define ACFT_VERTEXBUFFER_H_

namespace ACFT
{
	class VertexBuffer
	{
	public:
		VertexBuffer(unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
	private:
		unsigned int vb_id;
	};
}

#endif

