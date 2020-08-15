#include "PXpch.h"
#include "Pixelate/Audio/AudioListener.h"

#include "AL/al.h"

#include <GLM/glm/gtc/type_ptr.hpp>

#include "Pixelate/Platform/OpenAL/ALCommon.h"
#include "Pixelate/Debug/Instrumentor.h"

namespace Pixelate {


	// For now the orientation is fixed. If I decide to implement a 3D renderer
	// I might enable 3d audio.

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
		m_Gain = gain;
		ALCall(alListenerfv(AL_GAIN, &m_Gain));
	}

}