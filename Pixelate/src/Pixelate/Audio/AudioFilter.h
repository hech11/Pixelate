#pragma once

#include <string>

namespace Pixelate {


	// bandpass?
	enum class AudioFilterType { None = -1, LowPass, HighPass };


	struct AudioFilterSpecs {
		AudioFilterType Type;
		float Gain = 1.0f;
	};

	// TODO: how about implementing wet and dry channels?
	class AudioFilter
	{

		public :

			AudioFilter();
			AudioFilter(const AudioFilterSpecs& specs);

			~AudioFilter();

			void SetGain(float gain);
			const float GetGain() const { return m_Gain; }

			void SetType(const AudioFilterType& type);
			const AudioFilterType& GetType() const { return m_Type; }

			std::string ToString();

			const uint32_t GetFilterID() const { return m_FilterID; }
		private :
			uint32_t m_FilterID;
			AudioFilterType m_Type;

			float m_Gain = 1.0f;


	};

}