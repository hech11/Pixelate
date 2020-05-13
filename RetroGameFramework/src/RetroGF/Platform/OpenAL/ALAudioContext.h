#pragma once

#include "RetroGF/Audio/AudioContext.h"

namespace RGF {

	class ALAudioContext : public AudioContext {

		public:

			void Init() override;
			void Close() override;
	};

}