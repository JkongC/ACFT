#ifndef ACFT_INDEXBUFFER_H_
#define ACFT_INDEXBUFFER_H_

namespace ACFT 
{
	class IndexBuffer
	{
	public:
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const {
			return count;
		}
	private:
		unsigned int ib_id;
		unsigned int count;
	};
}

#endif

