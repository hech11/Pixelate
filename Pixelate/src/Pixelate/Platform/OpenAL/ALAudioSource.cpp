#include "PXpch.h"
#include "Pixelate/Audio/AudioSource.h"

#include "AL/al.h"
#include "Pixelate/Platform/OpenAL/ALCommon.h"


#include "Pixelate/Audio/AudioBuffer.h"

#include "Pixelate/Debug/Instrumentor.h"
namespace Pixelate {




	AudioSource::AudioSource() {

		PX_PROFILE_FUNCTION();
		ALCall(alGenSources(1, &m_AudioSourceID));

	}

	AudioSource::~AudioSource() {
		PX_PROFILE_FUNCTION();
		ALCall(alDeleteSources(1, &m_AudioSourceID));
	}


	void AudioSource::Play() {
		PX_PROFILE_FUNCTION();
		ALCall(alSourcePlay(m_AudioSourceID));

	}

	void AudioSource::Pause() {
		PX_PROFILE_FUNCTION();
		ALCall(alSourcePause(m_AudioSourceID));
	}


	void AudioSource::Stop() {
		PX_PROFILE_FUNCTION();
		ALCall(alSourceStop(m_AudioSourceID));

	}

	void AudioSource::SetLooping(bool loop) {
		PX_PROFILE_FUNCTION();
		m_IsLooping = loop;
		ALCall(alSourcei(m_AudioSourceID, AL_LOOPING, m_IsLooping));
	}


	void AudioSource::SetGain(float gain) {
		PX_PROFILE_FUNCTION();
		if (gain > 0.1f)
			gain = 0.1f;
		m_Gain = gain;
		ALCall(alSourcef(m_AudioSourceID, AL_GAIN, gain));

	}


	void AudioSource::SetPosition(const glm::vec3& position)
	{
		PX_PROFILE_FUNCTION();
		m_Position = position;
		ALCall(alSource3f(m_AudioSourceID, AL_POSITION, position.x, position.y, position.z));
	}

	void AudioSource::SetBufferData(const Ref<AudioBuffer>& buffer) {
		PX_PROFILE_FUNCTION();
		ALCall(alSourcei(m_AudioSourceID, AL_BUFFER, buffer->GetHandleID()));
	}


}