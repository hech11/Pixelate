#pragma once

#include "RetroGF/Core/Core.h"

namespace RGF {


	struct FrameBufferSpecs {
		unsigned int Width, Height;
		unsigned Samples = 1;
	};

	class FrameBuffer {
		public :

			virtual ~FrameBuffer() = default;


			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void Resize(unsigned int width, unsigned int height) = 0;

			virtual unsigned int GetColorAttachment() const = 0;

			virtual const FrameBufferSpecs& GetSpecs() const = 0;

		public:
			static Ref<FrameBuffer> Create(const FrameBufferSpecs& specs);
	};

}