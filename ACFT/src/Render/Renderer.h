#ifndef ACFT_RENDERER_H_
#define ACFT_RENDERER_H_

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"

namespace ACFT 
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer() = default;

		void Draw();
		void Flush();

	private:
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<IndexBuffer> ib;
		std::unique_ptr<VertexArray> va;
		std::unique_ptr<Shader> shader;
	};
}

#endif
