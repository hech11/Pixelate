#pragma once

#include "Buffer.h"
#include "RetroGF/Core/Core.h"



// VertexArray interface.

// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.

namespace RGF {

	class RGF_API VertexArray {

		public :

			virtual ~VertexArray() {}


			virtual void PushVertexBuffer(const Ref<RGF::VertexBuffer>& buffer) = 0;
			virtual void PushIndexBuffer(const Ref<RGF::IndexBuffer>& buffer) = 0 ;

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual const std::vector<Ref<RGF::VertexBuffer>>& GetVbos() const = 0;
			virtual const RGF::IndexBuffer& GetIbos() const = 0;


		public :
			static Ref<VertexArray> Create();

	};

}