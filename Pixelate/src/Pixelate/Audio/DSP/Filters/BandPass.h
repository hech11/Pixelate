#pragma once

#include "Pixelate/Audio/AudioFilter.h"


namespace Pixelate {

	class BandPass : public AudioFilter {


		public:
			BandPass();
			~BandPass();

			void SetGain(float gain) override;

			void SetLFGain(float gain);
			void SetHFGain(float gain);

			const float GetHFGain() const { return m_HFGain; }
			const float GetLFGain() const { return m_LFGain; }


			std::string ToString() override { return "BandPass"; }
		private:
			float m_HFGain = 1.0f, m_LFGain = 0.0f;

	};

}