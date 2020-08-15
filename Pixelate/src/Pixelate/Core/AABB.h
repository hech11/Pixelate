#pragma once


#include <Glm/glm.hpp>

namespace Pixelate {

	struct AABB {
		glm::vec3 Min = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Max = { 1.0f, 1.0f, 1.0f };

		AABB() 
			: Min({0.0f, 0.0f, 0.0f}), Max(1.0f, 1.0f, 1.0f)
		{

		}

		AABB(const glm::vec3& min, const glm::vec3& max)
			: Min(min), Max(max) {}

	};

}