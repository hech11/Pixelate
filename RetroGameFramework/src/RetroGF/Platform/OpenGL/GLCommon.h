#pragma once

#include "RetroGF/Log.h"
#include "RetroGF/Core.h"
#include <GLAD/include/glad.h>

// This is used to debug OpenGL code.

#ifdef RGF_DISTRIBUTE
#define GLCall(x) x
#else 
 // This is to be wrapped around every opengl code. Example : GLCall(glGenBuffers(1, ...)));

#define GLCall(x) RGF::GL::GLClearError();\
				x;\
RGF_ASSERT(RGF::GL::GLLogCall(#x, __FILE__, __LINE__), "");

#endif

namespace RGF { namespace GL {

	static void GLClearError() {
		while (glGetError() != GL_NO_ERROR);
	}
	static const char* GetGLErrorName(const GLenum& error) {
		switch (error) {
		case GL_INVALID_ENUM:
			return "GL_INVALID_ENUM";
		case GL_INVALID_OPERATION:
			return "GL_INVALID_OPERATION";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "GL_INVALID_FRAMEBUFFER_OPERATION";
		case GL_OUT_OF_MEMORY:
			return "GL_OUT_OF_MEMORY";
		default:
			return "no error listed!";
		}

		return "no error listed!";
	}

	static bool GLLogCall(const char* function, const char* file, int line) {
		if (GLenum error = glGetError()) { 
			RGF_CORE_ERROR("OpenGL Error! : %s", GetGLErrorName(error));
			return false;
		}

		return true;
	}
	

} }