#pragma once

#include "Texture.h"
#include <GLM/glm/glm.hpp>



// FrameBuffer interface.

// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.

namespace RGF {

	class FrameBuffer {

		public :

			virtual void Clear() const = 0;
			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			inline virtual unsigned int GetWidth() const = 0;
			inline virtual unsigned int GetHeight() const = 0;

			inline virtual unsigned int GetTexture() const = 0;
			inline virtual void SetClearColor(const glm::vec4& color) = 0;


		public :
			//TODO: width and height are not actually used...
			static FrameBuffer* Create(unsigned int width, unsigned int height);


	};

}