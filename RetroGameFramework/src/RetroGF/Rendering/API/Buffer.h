#pragma once

#include "RetroGF/Core.h"
#include "RetroGF/Rendering/API/VertexBufferLayout.h"



// VertexBuffer and IndexBuffer interface.
// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.


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

		public :
			static VertexBuffer* Create(VertexBufferUsage usage = VertexBufferUsage::Static);
	};


	class RGF_API IndexBuffer {

		public :

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;


			virtual unsigned int GetCount() const = 0;

		public :
			static IndexBuffer* Create(unsigned int* data, unsigned count);
			static IndexBuffer* Create(unsigned short* data, unsigned count);
			static IndexBuffer* Create(unsigned char* data, unsigned count);
	};


}