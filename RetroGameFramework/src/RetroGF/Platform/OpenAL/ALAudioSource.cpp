#include "RGFpch.h"
#include "RetroGF/Audio/AudioSource.h"

#include "AL/al.h"
#include "RetroGF/Platform/OpenAL/ALCommon.h"


#include "RetroGF/Audio/AudioBuffer.h"

namespace RGF {




	AudioSource::AudioSource() {

		ALCall(alGenSources(1, &m_AudioSourceID));

	}

	AudioSource::~AudioSource() {
		ALCall(alDeleteSources(1, &m_AudioSourceID));
	}


	void AudioSource::Play() {
		ALCall(alSourcePlay(m_AudioSourceID));

	}

	void AudioSource::Pause() {
		ALCall(alSourcePause(m_AudioSourceID));
	}


	void AudioSource::Stop() {
		ALCall(alSourceStop(m_AudioSourceID));

	}

	void AudioSource::SetLooping(bool loop) {
		m_IsLooping = loop;
		ALCall(alSourcei(m_AudioSourceID, AL_LOOPING, m_IsLooping));
	}


	void AudioSource::SetGain(float gain) {
		m_Gain = gain;
		ALCall(alSourcef(m_AudioSourceID, AL_GAIN, gain));

	}


	void AudioSource::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		ALCall(alSource3f(m_AudioSourceID, AL_POSITION, position.x, position.y, position.z));
	}

	void AudioSource::SetBufferData(const Ref<AudioBuffer>& buffer) {
		ALCall(alSourcei(m_AudioSourceID, AL_BUFFER, buffer->GetHandleID()));
	}



}