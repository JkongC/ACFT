#ifndef ACFT_VERTEXBUFFERLAYOUT_H_
#define ACFT_VERTEXBUFFERLAYOUT_H_

namespace ACFT
{
	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type) {
			switch (type)
			{
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
			case GL_FLOAT:			return 4;
			}
			__debugbreak();
			return 0;
		}
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout()
			:stride(0), count(0)
		{
		}


		~VertexBufferLayout() = default;

		template<typename T>
		void Push(unsigned int count) {
			static_assert(false);
		}

		template<>
		void Push<float>(unsigned int count) {
			elements.push_back({ GL_FLOAT, count, GL_FALSE });
			stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
		}

		template<>
		void Push<unsigned int>(unsigned int count) {
			elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
		}

		template<>
		void Push<unsigned char>(unsigned int count) {
			elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
		}

		inline const std::vector<VertexBufferElement>& GetElements() const {
			return elements;
		}

		inline unsigned GetStride() const {
			return stride;
		}

	private:
		std::vector<VertexBufferElement> elements;
		unsigned int count;
		unsigned int stride;
	};
}

#endif

