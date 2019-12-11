#pragma once


#include "Renderable.h"
#include "RetroGF/Core/Core.h"



// This contains classes for "Sprite"

namespace RGF {


	class RGF_API Sprite : public Renderable {

		public:
			Sprite(glm::vec3 position, glm::vec3 scale, glm::vec4 color);

			inline glm::i32vec4& GetTexCoords() { return m_TexCoords; }

			// TEMP: This only works with OPENGL
			void SetSpriteSampleCoords(const glm::i32vec4& UVTexCoords, const glm::vec2& textureDimensions);

		private :
			glm::i32vec4 m_TexCoords;

	};

}