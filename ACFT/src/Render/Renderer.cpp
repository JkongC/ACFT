#include "stdafx.h"

#include "Renderer.h"
#include "Log/Logger.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

void GLLogCall() {
    GLenum err;
    while ((err = glGetError())) {
		ACFT_GL_LOG("Error occured: {}", err);
    }
}

namespace ACFT
{
	Renderer::Renderer()
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	void Renderer::Draw()
	{

	}

	void Renderer::Flush()
	{

	}
}