#pragma once

#include <Glm/glm.hpp>

namespace Pixelate {


	// This uses xy, width and height rather than x and y min/max
	struct Rect {

		glm::uvec2 Position = { 0, 0 };
		glm::uvec2 Scale = { 0, 0 };



		Rect()
			: Position({ 0, 0 }), Scale(1, 1)
		{

		}

		Rect(const glm::uvec2& position, const glm::uvec2& scale)
			: Position(position), Scale(scale) {}


		bool operator==(const Rect& other) {
			return this->Position == other.Position && this->Scale == other.Scale;
		}
	};


}
