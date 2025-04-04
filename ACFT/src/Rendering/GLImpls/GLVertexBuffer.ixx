module;

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <glew.h>
#include <glfw3.h>

export module Renderer:GLVertexBuffer;

import <vector>;
import Vertex;

namespace GLImplementations
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		VertexBuffer(size_t max_size);
		~VertexBuffer();

		void Init(size_t max_size);

		void Bind() const;
		void Unbind() const;
		bool Submit(const ACFT::Vertex& vtx);
		void Clear();

		inline size_t GetCurrentBufferSize() const { return m_CurrentSize; }
		inline size_t GetRemainingSize() const { return m_MaxBufferSize - m_CurrentSize; }
		inline size_t GetMaxBufferSize() const { return m_MaxBufferSize; }
		inline size_t GetCurrentVertexCount() const { return m_CurrentVertexCount; }

	private:
		template<typename Attribute>
		void PushSingleAttribute(const ACFT::Vertex& vtx);

	private:
		unsigned int m_BufferID;
		size_t m_MaxBufferSize{ 0 };
		size_t m_CurrentSize{ 0 };
		size_t m_CurrentVertexCount{ 0 };
	};

	struct VertexBufferElement
	{
		unsigned int type;
		size_t count;
		unsigned char normalized;

		static size_t GetSizeOfType(unsigned int type) {
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

		inline size_t GetStride() const {
			return m_Stride;
		}

	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Count;
		size_t m_Stride;
	};
}