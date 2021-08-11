#include "PXpch.h"
#include "Pixelate/Audio/AudioListener.h"

#include "AL/al.h"

#include <GLM/glm/gtc/type_ptr.hpp>

#include "Pixelate/Platform/OpenAL/ALCommon.h"
#include "Pixelate/Debug/Instrumentor.h"

namespace Pixelate {



	AudioListener::AudioListener() {
		PX_PROFILE_FUNCTION();
		ALfloat ori[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
		ALCall(alListenerfv(AL_ORIENTATION, ori));
	}

	void AudioListener::SetPosition(const glm::vec3& position) {
		PX_PROFILE_FUNCTION();
		m_Position = position;
		ALCall(alListenerfv(AL_POSITION, glm::value_ptr(m_Position)));
	}

	void AudioListener::SetGain(float gain)
	{
		PX_PROFILE_FUNCTION();
		Audio::GetGlobalMixer()->GetMasterGroup()->Gain = gain;
		ALCall(alListenerfv(AL_GAIN, &gain));
	}

	void AudioListener::SetSpatial(bool value) {
		m_IsSpatial = value;
		if (m_IsSpatial) {
			ALCall(alDistanceModel(AL_LINEAR_DISTANCE));
		}
		else {
			ALCall(alDistanceModel(AL_NONE));
		}
	}


}