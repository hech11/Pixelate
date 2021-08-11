#pragma once

#include "Pixelate/Audio/AudioFilter.h"

namespace Pixelate {


	class LowPass : public AudioFilter {

		public:
			LowPass();
			~LowPass();

			void SetGain(float gain) override;

			void SetHFGain(float gain);
			const float GetHFGain() const { return m_HFGain; }

			std::string ToString() override { return "LowPass"; }
		private :
			float m_HFGain = 0.0f;
	};

}