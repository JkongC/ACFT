#pragma once

#define GLCall(x) GLClearError();\
					x;\
					GLLogCall()

inline void GLClearError()
{
	//while (glGetError() != GL_NO_ERROR);
}

inline void GLLogCall() {
	/*GLenum err;
	while (err = glGetError()) {
		ACFT_GL_LOG("Error occured: {}", err);

#if 1
		__debugbreak();
#endif
	}*/
}