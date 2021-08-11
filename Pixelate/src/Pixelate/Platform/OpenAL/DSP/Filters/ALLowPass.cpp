#include "PXpch.h"
#include "Pixelate/Audio/DSP/Filters/LowPass.h"

#include "Pixelate/Platform/OpenAL/ALCommon.h"

#include <OpenAL_Soft/include/AL/alc.h>
#include <OpenAL_Soft/include/AL/al.h>

#define AL_ALEXT_PROTOTYPES
#include <OpenAL_Soft/include/AL/efx.h>

namespace Pixelate {


	LowPass::LowPass() {
		ALCall(alGenFilters(1, &m_FilterID));

		m_Type = AudioFilterType::LowPass;
		ALCall(alFilteri(m_FilterID, AL_FILTER_TYPE, AL_FILTER_LOWPASS));
	}
	
	LowPass::~LowPass() {
		ALCall(alDeleteFilters(1, &m_FilterID));
	}


	void LowPass::SetGain(float gain) {

		m_Gain = gain;
		ALCall(alFilterf(m_FilterID, AL_LOWPASS_GAIN, m_Gain));

	}

	void LowPass::SetHFGain(float gain) {
		m_HFGain = gain;
		ALCall(alFilterf(m_FilterID, AL_LOWPASS_GAINHF, m_HFGain));

	}



}