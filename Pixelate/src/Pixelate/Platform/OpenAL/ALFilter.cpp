#include "PXpch.h"

#include "Pixelate/Audio/AudioFilter.h"

#include <OpenAL_Soft/include/AL/al.h>

#define AL_ALEXT_PROTOTYPES
#include <OpenAL_Soft/include/AL/efx.h>
#include "ALCommon.h"

namespace Pixelate {

	namespace Utils {

		static std::string FilterToString(const AudioFilterType& filter) {
			switch (filter)
			{
				case AudioFilterType::LowPass: return "Low-pass"; break;
				case AudioFilterType::HighPass: return "High-pass"; break;
			}

			return "Could not deduce type!";
		}

		static uint32_t FilterTypeToAlFilterType(AudioFilterType& type) {
			switch (type)
			{
				case AudioFilterType::LowPass: return AL_FILTER_LOWPASS; break;
				case AudioFilterType::HighPass: return  AL_FILTER_HIGHPASS; break;
			}

			return 0;
		}

		static uint32_t GetFilterGain(uint32_t filterType) {
			switch (filterType)
			{
				case AL_FILTER_LOWPASS: return AL_LOWPASS_GAIN; break;
				case AL_FILTER_HIGHPASS: return AL_HIGHPASS_GAIN; break;
			}

			return 0;
		}

	}


	AudioFilter::AudioFilter() {
		ALCall(alGenFilters(1, &m_FilterID));

	}
	
	AudioFilter::AudioFilter(const AudioFilterSpecs& specs) {
		ALCall(alGenFilters(1, &m_FilterID));

		SetType(specs.Type);
		SetGain(specs.Gain);

	}

	AudioFilter::~AudioFilter() {
		ALCall(alDeleteFilters(1, &m_FilterID));
	}


	void AudioFilter::SetGain(float gain) {

		uint32_t type = Utils::FilterTypeToAlFilterType(m_Type);
		uint32_t gainType = Utils::GetFilterGain(type);

		m_Gain = gain;
		ALCall(alFilterf(m_FilterID, gainType, m_Gain));

	}

	void AudioFilter::SetType(const AudioFilterType& type) {
		m_Type = type;

		uint32_t filterType = Utils::FilterTypeToAlFilterType(m_Type);
		ALCall(alFilteri(m_FilterID, AL_FILTER_TYPE, filterType));


	}
	std::string AudioFilter::ToString() {
		return Utils::FilterToString(m_Type);
	}




}
