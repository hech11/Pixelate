#include "PXpch.h"
#include "Pixelate/Audio/DSP/Filters/BandPass.h"


#include "Pixelate/Platform/OpenAL/ALCommon.h"


#include <OpenAL_Soft/include/AL/alc.h>
#include <OpenAL_Soft/include/AL/al.h>

#define AL_ALEXT_PROTOTYPES
#include <OpenAL_Soft/include/AL/efx.h>

namespace Pixelate {


	BandPass::BandPass() {
		ALCall(alGenFilters(1, &m_FilterID));

		m_Type = AudioFilterType::BandPass;
		ALCall(alFilteri(m_FilterID, AL_FILTER_TYPE, AL_FILTER_BANDPASS));
	}

	BandPass::~BandPass() {
		ALCall(alDeleteFilters(1, &m_FilterID));
	}


	void BandPass::SetGain(float gain) {

		m_Gain = gain;
		ALCall(alFilterf(m_FilterID, AL_BANDPASS_GAIN, m_Gain));

	}

	void BandPass::SetLFGain(float gain) {
		m_LFGain = gain;
		ALCall(alFilterf(m_FilterID, AL_BANDPASS_GAINLF, m_LFGain));

	}

	void BandPass::SetHFGain(float gain) {
		m_HFGain = gain;
		ALCall(alFilterf(m_FilterID, AL_BANDPASS_GAINHF, m_LFGain));

	}

}