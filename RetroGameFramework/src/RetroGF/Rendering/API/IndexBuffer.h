#pragma once

#include "RetroGF/Core.h"


// IndexBuffer interface.

// The 'Create' method will decide depending on the API choice should IndexBuffer choose
// OpenGL, Directx 11 or 12, vulkan etc... implementation of IndexBuffer.


namespace RGF {

	class RGF_API IndexBuffer {

		public :

			virtual ~IndexBuffer(){}

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;


			virtual unsigned int GetCount() const = 0;

			static IndexBuffer* Create(unsigned int* data, unsigned count);
			static IndexBuffer* Create(unsigned short* data, unsigned count);
			static IndexBuffer* Create(unsigned char* data, unsigned count);
	};

}