module;

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <glew.h>
#include <glfw3.h>

export module Renderer:GLVertexBuffer;

import <vector>;

namespace GLImplementations
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

	private:
		unsigned int m_BufferID;
	};

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
			:m_Stride(0), m_Count(0)
		{
		}

		~VertexBufferLayout() = default;

		template<typename T>
		void Push(unsigned int count) {
			static_assert(false);
		}

		template<>
		void Push<float>(unsigned int count) {
			m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
		}

		template<>
		void Push<unsigned int>(unsigned int count) {
			m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
		}

		template<>
		void Push<unsigned char>(unsigned int count) {
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
		}

		inline const std::vector<VertexBufferElement>& GetElements() const {
			return m_Elements;
		}

		inline unsigned GetStride() const {
			return m_Stride;
		}

	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Count;
		unsigned int m_Stride;
	};
}