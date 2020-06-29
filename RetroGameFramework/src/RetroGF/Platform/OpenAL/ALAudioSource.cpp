#include "RGFpch.h"
#include "RetroGF/Audio/AudioSource.h"

#include "AL/al.h"
#include "RetroGF/Platform/OpenAL/ALCommon.h"


#include "RetroGF/Audio/AudioBuffer.h"

#include "RetroGF/Debug/Instrumentor.h"
namespace RGF {




	AudioSource::AudioSource() {

		RGF_PROFILE_FUNCTION();
		ALCall(alGenSources(1, &m_AudioSourceID));

	}

	AudioSource::~AudioSource() {
		RGF_PROFILE_FUNCTION();
		ALCall(alDeleteSources(1, &m_AudioSourceID));
	}


	void AudioSource::Play() {
		RGF_PROFILE_FUNCTION();
		ALCall(alSourcePlay(m_AudioSourceID));

	}

	void AudioSource::Pause() {
		RGF_PROFILE_FUNCTION();
		ALCall(alSourcePause(m_AudioSourceID));
	}


	void AudioSource::Stop() {
		RGF_PROFILE_FUNCTION();
		ALCall(alSourceStop(m_AudioSourceID));

	}

	void AudioSource::SetLooping(bool loop) {
		RGF_PROFILE_FUNCTION();
		m_IsLooping = loop;
		ALCall(alSourcei(m_AudioSourceID, AL_LOOPING, m_IsLooping));
	}


	void AudioSource::SetGain(float gain) {
		RGF_PROFILE_FUNCTION();
		m_Gain = gain;
		ALCall(alSourcef(m_AudioSourceID, AL_GAIN, gain));

	}


	void AudioSource::SetPosition(const glm::vec3& position)
	{
		RGF_PROFILE_FUNCTION();
		m_Position = position;
		ALCall(alSource3f(m_AudioSourceID, AL_POSITION, position.x, position.y, position.z));
	}

	void AudioSource::SetBufferData(const Ref<AudioBuffer>& buffer) {
		RGF_PROFILE_FUNCTION();
		ALCall(alSourcei(m_AudioSourceID, AL_BUFFER, buffer->GetHandleID()));
	}



}