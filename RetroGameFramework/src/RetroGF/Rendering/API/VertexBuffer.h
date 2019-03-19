#pragma once

#include "RetroGF/Core.h"
#include "RetroGF/Rendering/API/VertexBufferLayout.h"

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



			virtual void Bind() = 0;
			virtual void Unbind() = 0;

			static VertexBuffer* Create(VertexBufferUsage usage = VertexBufferUsage::Static);
	};

}