#ifndef ACFT_INDEXBUFFER_H_
#define ACFT_INDEXBUFFER_H_

namespace ACFT 
{
	class IndexBuffer
	{
	public:

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const {
			return count;
		}

		inline const void* GetRawBuffer() const { return local_buffer.data(); }

	private:
		unsigned int ib_id;
		unsigned int count;

		std::array<unsigned int, maxVerteciesPerDraw * 2> local_buffer;

	private:
		friend class RenderSystem;
		IndexBuffer();
		IndexBuffer(std::initializer_list<unsigned int> template_index, bool fill_to_full);
	};
}

#endif

