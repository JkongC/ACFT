#ifndef ACFT_VERTEXARRAY_H_
#define ACFT_VERTEXARRAY_H_

namespace ACFT
{
	class VertexBufferLayout;

	class VertexArray
	{
	public:
		VertexArray();

		void Bind() const;

		void Unbind() const;

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
		{
			Bind();
			vb.Bind();
			const auto& elements = layout.GetElements();
			unsigned offset = 0;
			for (unsigned int i = 0; i < elements.size(); i++) {
				const auto& element = elements[i];
				GLCall(glEnableVertexAttribArray(i));
				GLCall(glVertexAttribPointer(i, element.count, element.type,
					element.normalized, layout.GetStride(), (const void*)offset));
				offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
			}
		}
	private:
		unsigned int va_id;
	};
}

#endif

