#pragma once

#include "RetroGF/Core.h"


namespace RGF {

	class RGF_API IndexBuffer {

		public :
			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;


			virtual unsigned int GetCount() const = 0;

			static IndexBuffer* Create(unsigned int* data, unsigned count);
			static IndexBuffer* Create(unsigned short* data, unsigned count);
			static IndexBuffer* Create(unsigned char* data, unsigned count);
	};

}