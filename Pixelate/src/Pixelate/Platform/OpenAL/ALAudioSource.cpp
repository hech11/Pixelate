#include "PXpch.h"
#include "Pixelate/Audio/AudioSource.h"

#include "Pixelate/Audio/AudioMixer.h"

#include "AL/al.h"
#include "AL/efx.h"

#include "Pixelate/Platform/OpenAL/ALCommon.h"


#include "Pixelate/Audio/AudioBuffer.h"

#include "Pixelate/Debug/Instrumentor.h"
namespace Pixelate {




	AudioSource::AudioSource() {

		PX_PROFILE_FUNCTION();
		ALCall(alGenSources(1, &m_AudioSourceID));

		m_MixerGroup = Audio::GetGlobalMixer()->GetMasterGroup();
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
		m_Gain = gain;

		if (m_Gain > 1.0f)
			m_Gain = 1.0f;

		if (m_Gain < 0.0f)
			m_Gain = 0.0f;

		ALCall(alSourcef(m_AudioSourceID, AL_GAIN, m_Gain * m_MixerGroup->Gain));

	}


	void AudioSource::SetPosition(const glm::vec3& position)
	{
		PX_PROFILE_FUNCTION();
		m_Position = position;
		ALCall(alSource3f(m_AudioSourceID, AL_POSITION, position.x, position.y, position.z));
	}


	void AudioSource::ApplyFilterChanges() {
		if(m_LowpassFilter)
			ALCall(alSourcei(m_AudioSourceID, AL_DIRECT_FILTER, m_LowpassFilter->GetFilterID()));
		if (m_BandpassFilter)
			ALCall(alSourcei(m_AudioSourceID, AL_DIRECT_FILTER, m_BandpassFilter->GetFilterID()));
		if (m_HighpassFilter)
			ALCall(alSourcei(m_AudioSourceID, AL_DIRECT_FILTER, m_HighpassFilter->GetFilterID()));
	}


	const Ref<AudioBuffer>& AudioSource::GetBufferData() const
	{
		return m_AudioBuffer;
	}
	void AudioSource::SetBufferData(const Ref<AudioBuffer>& buffer) {
		PX_PROFILE_FUNCTION();
		m_AudioBuffer = buffer;
		ALCall(alSourcei(m_AudioSourceID, AL_BUFFER, buffer->GetHandleID()));
	}



	void AudioSource::SetPitch(float pitch)
	{
		m_Pitch = pitch;
		ALCall(alSourcef(m_AudioSourceID, AL_PITCH, m_Pitch));
	}


	void AudioSource::ShouldMute(bool mute)
	{
		if (mute) {
			ALCall(alSourcef(m_AudioSourceID, AL_GAIN, 0.0f));
			m_State = m_State | AudioMixerStates::Mute;
		}
		else {
			ALCall(alSourcef(m_AudioSourceID, AL_GAIN, m_Gain * m_MixerGroup->Gain));
			m_State = m_State & (~(AudioMixerStates::Mute));
		}

	}

	void AudioSource::PlayOnAwake(bool awake) {
		m_PlayOnAwake = awake;
	}

	void AudioSource::ShouldBypassEffects(bool bypass) {
		if (bypass) {
			m_State = m_State | AudioMixerStates::Bypass;
		}
		else {
			m_State = m_State & (~(AudioMixerStates::Bypass));
		}
		
	}


}