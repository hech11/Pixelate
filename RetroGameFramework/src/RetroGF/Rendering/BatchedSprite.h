#pragma once


#include "Renderable.h"
#include "RetroGF/Core.h"

namespace RGF {

	class RGF_API BatchedSprite : public Renderable {

	public:
		BatchedSprite(glm::vec3 position, glm::vec3 scale, glm::vec4 color)
			: Renderable(position, scale, color)
		{
			
		}


	};

}