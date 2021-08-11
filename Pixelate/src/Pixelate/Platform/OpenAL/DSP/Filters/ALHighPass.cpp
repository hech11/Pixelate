#include "PXpch.h"

#include "Pixelate/Audio/DSP/Filters/HighPass.h"

#include "Pixelate/Platform/OpenAL/ALCommon.h"

#include <OpenAL_Soft/include/AL/alc.h>
#include <OpenAL_Soft/include/AL/al.h>

#define AL_ALEXT_PROTOTYPES
#include <OpenAL_Soft/include/AL/efx.h>

namespace Pixelate {


	HighPass::HighPass() {
		ALCall(alGenFilters(1, &m_FilterID));

		m_Type = AudioFilterType::HighPass;
		ALCall(alFilteri(m_FilterID, AL_FILTER_TYPE, AL_FILTER_HIGHPASS));
	}

	HighPass::~HighPass() {
		ALCall(alDeleteFilters(1, &m_FilterID));
	}


	void HighPass::SetGain(float gain) {

		m_Gain = gain;
		ALCall(alFilterf(m_FilterID, AL_HIGHPASS_GAIN, m_Gain));

	}

	void HighPass::SetLFGain(float gain) {
		m_LFGain = gain;
		ALCall(alFilterf(m_FilterID, AL_HIGHPASS_GAINLF, m_LFGain));

	}


	

}