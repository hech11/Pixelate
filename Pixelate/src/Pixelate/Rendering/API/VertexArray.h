#pragma once

#include "Buffer.h"
#include "Pixelate/Core/Core.h"



// VertexArray interface.

// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.

namespace Pixelate {

	class PX_API VertexArray {

		public :

			virtual ~VertexArray() {}


			virtual void PushVertexBuffer(const Ref<Pixelate::VertexBuffer>& buffer) = 0;
			virtual void PushIndexBuffer(const Ref<Pixelate::IndexBuffer>& buffer) = 0 ;

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual const std::vector<Ref<Pixelate::VertexBuffer>>& GetVbos() const = 0;
			virtual const Pixelate::IndexBuffer& GetIbos() const = 0;


		public :
			static Ref<VertexArray> Create();

	};

}