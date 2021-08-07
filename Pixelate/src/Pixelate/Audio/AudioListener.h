#pragma once

#include "Pixelate/Core/Core.h"
#include <GLM/glm/glm.hpp>

namespace Pixelate {

	class AudioListener {
		public :

			AudioListener();

			void SetPosition(const glm::vec3& position);
			void SetGain(float gain);

			void SetSpatial(bool value);
			bool IsSpatial() const { return m_IsSpatial; }

		private :
			glm::vec3 m_Position;
			bool m_IsSpatial = false;

	};

}