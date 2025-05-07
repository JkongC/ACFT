#pragma once

#if defined(ACFT_DEBUG)

#define GLCall(x) GLClearError();\
				x;\
				GLLogCall()

#else

#define GLCall(x) x

#endif

inline void GLClearError()
{
	//while (glGetError() != GL_NO_ERROR);
}

inline void GLLogCall() {
	GLenum err;
	while (err = glGetError()) {
		ACFT_GL_LOG("Error occured: {}", err);

#if 1
		__debugbreak();
#endif
	}
}