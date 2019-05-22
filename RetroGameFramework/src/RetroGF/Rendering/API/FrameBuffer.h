#pragma once

#include "Texture.h"
#include <GLM/glm/glm.hpp>

namespace RGF {

	class FrameBuffer {

		public :

			static FrameBuffer* Create(unsigned int width, unsigned int height);


			virtual void Clear() const = 0;
			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			inline virtual unsigned int GetWidth() const = 0;
			inline virtual unsigned int GetHeight() const = 0;

			inline virtual unsigned int GetTexture() const = 0;
			inline virtual void SetClearColor(const glm::vec4& color) = 0;

	};

}