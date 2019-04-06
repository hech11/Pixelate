#pragma once

#include "RetroGF/Core.h"
#include "RetroGF/Rendering/API/VertexBufferLayout.h"



// VertexBuffer interface.

// The 'Create' method will decide depending on the API choice should VertexBuffer choose
// OpenGL, Directx 11 or 12, vulkan etc... implementation of VertexBuffer.



namespace RGF {
	enum class VertexBufferUsage {
		None = 0,
		Static,
		Dynamic
	};

	class RGF_API VertexBuffer {
		public :
			

			virtual void SetData(unsigned int size, const void* data) = 0;
			virtual void Resize(unsigned int size) = 0;
			virtual void SetLayout(const RGF::VertexBufferLayout& layout) = 0;



			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			static VertexBuffer* Create(VertexBufferUsage usage = VertexBufferUsage::Static);
	};

}