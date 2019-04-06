#pragma once

#include "VertexBuffer.h"
#include "RetroGF/Core.h"



// VertexArray interface.

// The 'Create' method will decide depending on the API choice should VertexArray choose
// OpenGL, Directx 11 or 12, vulkan etc... implementation of VertexArray.


namespace RGF {

	class RGF_API VertexArray {

		public :
			virtual void PushBuffer(RGF::VertexBuffer* buffer) = 0;

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void Draw(unsigned int count) = 0;

			static VertexArray* Create();
	};

}