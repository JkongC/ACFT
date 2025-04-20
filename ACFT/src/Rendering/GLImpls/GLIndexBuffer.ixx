module;

#include <glew.h>
#include <glfw3.h>

export module Renderer:GLIndexBuffer;

import <vector>;

import Tesselator;
import :GLIndexBuilder;

namespace GLImplementations
{
	using namespace ACFT;
	class IndexBuffer
	{
	public:
		IndexBuffer();
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&&) = default;
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		template<Primitive mode>
		void GenerateByMode(unsigned int max_primitive_count)
		{
			Bind();
			std::vector<unsigned int> indices = IndexBuilder::Create(mode)
				.IndexCount(IndexCountPerPrimitive(mode) * max_primitive_count)
				.Build();
			this->m_Count = indices.size();
			this->m_Size = this->m_Count * sizeof(unsigned int);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_Size, indices.data(), GL_STATIC_DRAW);

			Unbind();
		}

		template<Primitive mode>
		void GenerateByMode()
		{
			Bind();
			std::vector<unsigned int> indices = IndexBuilder::GetDefaultBuild(mode);
			this->m_Count = indices.size();
			this->m_Size = this->m_Count * sizeof(unsigned int);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_Size, indices.data(), GL_STATIC_DRAW);

			Unbind();
		}

		inline size_t GetCount() const { return m_Count; }
		inline size_t GetSize() const { return m_Size; }

	private:
		unsigned int m_BufferID;
		size_t m_Count{0};
		size_t m_Size{0};
		bool m_Dynamic{false};
	};
}