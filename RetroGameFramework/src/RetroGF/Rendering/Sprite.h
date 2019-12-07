#pragma once


#include "Renderable.h"
#include "RetroGF/Core/Core.h"



// This contains classes for "Sprite"

namespace RGF {


	class RGF_API Sprite : public Renderable {

		public:
			Sprite(glm::vec3 position, glm::vec3 scale, glm::vec4 color);

	};

}