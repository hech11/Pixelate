#pragma once

#include "VertexBuffer.h"
#include "RetroGF/Core.h"


namespace RGF {

	class RGF_API VertexArray {

		public :
			virtual void PushBuffer(RGF::VertexBuffer* buffer) = 0;

			virtual void Bind() = 0;
			virtual void Unbind() = 0;

			virtual void Draw(unsigned int count) = 0;

			static VertexArray* Create();
	};

}