#pragma once

#include "Pixelate/Audio/AudioFilter.h"


namespace Pixelate {

	class HighPass : public AudioFilter {
		public:
			HighPass();
			~HighPass();

			void SetGain(float gain) override;

			void SetLFGain(float gain);
			const float GetLFGain() const { return m_LFGain; }


			std::string ToString() override { return "HighPass"; }
		private:
			float m_LFGain = 0.0f;


	};

}