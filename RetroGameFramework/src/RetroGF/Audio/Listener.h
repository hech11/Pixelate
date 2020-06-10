#pragma once

#include "RetroGF/Core/Core.h"
#include <GLM/glm/glm.hpp>

namespace RGF {

	class Listener {
		public :
			virtual void SetPosition(const glm::vec3& position) = 0;
			virtual void EnableSpatial(bool enable) = 0;

			virtual void SetGain(float gain) = 0;

		public :
			static Ref<Listener> Create();

		protected :
			glm::vec3 m_Position;
			float m_Gain;

	};

}