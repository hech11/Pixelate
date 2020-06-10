#include "RGFpch.h"
#include "ALListener.h"

#include "AL/al.h"

#include <GLM/glm/gtc/type_ptr.hpp>

#include "RetroGF/Platform/OpenAL/ALCommon.h"

namespace RGF {



	ALListener::ALListener() {
		ALfloat ori[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
		ALCall(alListenerfv(AL_ORIENTATION, ori));
	}

	ALListener::~ALListener() {

	}

	void ALListener::SetPosition(const glm::vec3& position) {
		m_Position = position;
		ALCall(alListenerfv(AL_POSITION, glm::value_ptr(m_Position)));
	}

	void ALListener::EnableSpatial(bool enable) {

	}

	void ALListener::SetGain(float gain)
	{
		m_Gain = gain;
		ALCall(alListenerfv(AL_GAIN, &m_Gain));
	}

}