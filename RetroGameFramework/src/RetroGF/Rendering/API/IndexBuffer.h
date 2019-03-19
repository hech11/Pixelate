#pragma once

#include "RetroGF/Core.h"


namespace RGF {

	class RGF_API IndexBuffer {

		public :
			virtual void Bind() = 0;
			virtual void Unbind() = 0;


			virtual unsigned int GetCount() const = 0;

			static IndexBuffer* Create(unsigned int* data, unsigned count);
			static IndexBuffer* Create(unsigned short* data, unsigned count);
			static IndexBuffer* Create(unsigned char* data, unsigned count);
	};

}