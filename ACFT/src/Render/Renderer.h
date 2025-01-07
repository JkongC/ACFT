#ifndef ACFT_RENDERER_H_
#define ACFT_RENDERER_H_

void GLClearError();
void GLLogCall();

#define GLCall(x) GLClearError();\
					x;\
					GLLogCall()

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace ACFT 
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Draw();
		void Flush();
	private:

	};
}

#endif
