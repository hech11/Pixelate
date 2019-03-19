#include "RGFpch.h"
#include "VertexBufferLayout.h"


#include "GLAD/include/glad.h"
namespace RGF {

	unsigned char BufferElement::ConvertGLTypeToBytes(unsigned int type) {

		// If OpenGL
		switch (type) {
			case GL_UNSIGNED_INT:
				return sizeof(unsigned int);
			case GL_UNSIGNED_SHORT:
				return sizeof(unsigned short);
			case GL_UNSIGNED_BYTE:
				return sizeof(unsigned char);
			case GL_FLOAT:
				return sizeof(float);
		}

		RGF_CORE_CRIT("Type does not exist!\n");
		return 0;
	}
	unsigned int BufferElement::ConvertBytesToGLType(const BufferElement::GLTYPES& type) {
		// If OpenGL
		switch (type) {
			case GLTYPES::Unsigned_Int:
				return GL_UNSIGNED_INT;
			case GLTYPES::Unsigned_Short:
				return GL_UNSIGNED_SHORT;
			case GLTYPES::Unsigned_Byte:
				return GL_UNSIGNED_BYTE;
			case GLTYPES::Float:
				return GL_FLOAT;
		}

		RGF_CORE_CRIT("Type does not exist!\n");
		return 0;
	}




}