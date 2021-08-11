#pragma once

#include <string>

namespace Pixelate {


	enum class AudioFilterType { None = -1, BandPass, LowPass, HighPass };


	struct AudioFilterSpecs {
		AudioFilterType Type;
		float Gain = 1.0f;
	};


	class AudioFilter
	{

		public :

			virtual ~AudioFilter(){}

			virtual void SetGain(float gain) = 0;

			const float GetGain() const { return m_Gain; }

			const AudioFilterType& GetType() const { return m_Type; }

			virtual std::string ToString() = 0;

			const uint32_t GetFilterID() const { return m_FilterID; }

		protected :
			uint32_t m_FilterID;
			AudioFilterType m_Type;

			float m_Gain = 1.0f;


	};

}