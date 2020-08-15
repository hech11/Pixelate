#pragma once

#include "Pixelate/Core/Core.h"
#include <Glm/glm.hpp>


//TODO: I need to create a frame buffer manager or pool at some point
namespace Pixelate {


	struct FrameBufferSpecs {
		unsigned int Width, Height;
		glm::vec4 ClearColor;
		unsigned Samples = 1;
	};

	class FrameBuffer { 
		public :

			virtual ~FrameBuffer() = default;


			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void Resize(unsigned int width, unsigned int height) = 0;

			virtual unsigned int GetColorAttachment() const = 0;
			virtual unsigned int GetRendererID() const = 0;

			virtual const FrameBufferSpecs& GetSpecs() const = 0;

		public:
			static Ref<FrameBuffer> Create(const FrameBufferSpecs& specs);
	};

}