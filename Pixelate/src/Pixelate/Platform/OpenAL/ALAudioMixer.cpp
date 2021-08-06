#include "PXpch.h"
#include "Pixelate/Audio/AudioMixer.h"


#include <OpenAL_Soft/include/AL/al.h>

namespace Pixelate {

	AudioMixer::AudioMixer() {

		m_MasterGroup = CreateRef<AudioMixerGroup>();
		m_MasterGroup->DebugName = "Master";
		m_MasterGroup->Gain = 1.0f;

	}

}