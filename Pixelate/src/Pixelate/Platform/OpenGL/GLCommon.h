#pragma once

#include "Pixelate/Core/Log.h"
#include "Pixelate/Core/Core.h"
#include <glad/glad.h>

// This is used to debug OpenGL code.

#define USE_OLD_DEBUGGING_OUTPUT 0


#if USE_OLD_DEBUGGING_OUTPUT
#ifdef PX_DISTRIBUTE
	#define GLCall(x) x
#else 

#define GLCall(x) Pixelate::GL::GLClearError(); x; PX_ASSERT(Pixelate::GL::GLLogCall(#x, __FILE__, __LINE__), "")
#endif
#else
	#define GLCall(x) x
#endif


namespace Pixelate {
	namespace GL {

#if USE_OLD_DEBUGGING_OUTPUT


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
				PX_CORE_ERROR("OpenGL Error! : %s", GetGLErrorName(error));
				return false;
			}

			return true;
		}
#else


		static void APIENTRY OpenGLDebugOuput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userData)
		{
			std::string _source, _type, _severity;
			bool discardLogMessage = false;

			switch (source)
			{
			case GL_DEBUG_SOURCE_API:
				_source = "API";
				break;
			case GL_DEBUG_SOURCE_APPLICATION:
				_source = "Application";
				break;

			case GL_DEBUG_SOURCE_OTHER:
				_source = "Other";
				break;

			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				_source = "Shader compiler";
				break;

			case GL_DEBUG_SOURCE_THIRD_PARTY:
				_source = "Third party";
				break;
			default:
				_source = "Unknown (not listed)";
				break;
			}

			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR:
				_type = "Error";
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				_type = "Deprecated behavior";
				break;

			case GL_DEBUG_TYPE_OTHER:
				_type = "Other";
				break;

			case GL_DEBUG_TYPE_PORTABILITY:
				_type = "Portability";
				break;

			case GL_DEBUG_TYPE_PERFORMANCE:
				_type = "Performance";
				break;
			default:
				_type = "Unknown (not listed)";
				break;
			}

			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				_severity = "H Severity";
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				_severity = "M Severity";
				break;
			case GL_DEBUG_SEVERITY_LOW:
				_severity = "L Severity";
				break;
			default:
				discardLogMessage = true;
				_severity = "Unknown (not listed)";
				break;
			}

			if(!discardLogMessage)
				PX_CORE_ERROR("[OpenGL][%s][%s][%d][%s]: %s\n", _source.c_str(), _type.c_str(), id, _severity.c_str(), message);


		}
#endif

		static void InitDebugCallback() {
			#if !USE_OLD_DEBUGGING_OUTPUT
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(OpenGLDebugOuput, 0);
			#endif
		}

	}
}

