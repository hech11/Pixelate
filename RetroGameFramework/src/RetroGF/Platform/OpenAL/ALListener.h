#pragma once

#include "RetroGF/Audio/Listener.h"


namespace RGF {

	class ALListener : public Listener {
		public :
			ALListener();
			virtual ~ALListener();

			void SetPosition(const glm::vec3& position) override;
			void EnableSpatial(bool enable) override;

			void SetGain(float gain) override;
	};

}