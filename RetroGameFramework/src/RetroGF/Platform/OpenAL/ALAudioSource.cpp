#include "RGFpch.h"
#include "ALAudioSource.h"

#include "AL/al.h"
#include "RetroGF/Platform/OpenAL/ALCommon.h"


#include "RetroGF/Audio/WavFileHeader.h"

namespace RGF {

	ALAudioSource::ALAudioSource(const AudioSourceSpecification& specs)
	: m_Specification(specs)
	{
		ALCall(alGenBuffers(1, &m_AudioBufferID));


		int channels, sampleRate, bps, size;
		void* wavData = WavFileHeader::LoadData(m_Specification.filepath, &channels, &sampleRate, &bps
		, &size);

		ALCall(alBufferData(m_AudioBufferID, DeduceALFormat(channels, bps),
			wavData, size, sampleRate));


		ALCall(alGenSources(1, &m_AudioSourceID));
		ALCall(alSourcei(m_AudioSourceID, AL_BUFFER, m_AudioBufferID));

	}

	ALAudioSource::~ALAudioSource() {
		ALCall(alDeleteSources(1, &m_AudioSourceID));
		ALCall(alDeleteBuffers(1, &m_AudioBufferID));
	}


	void ALAudioSource::Play() {
		ALCall(alSourcePlay(m_AudioSourceID));

	}

	void ALAudioSource::Pause() {
		ALCall(alSourcePause(m_AudioSourceID));
	}


	void ALAudioSource::Stop() {
		ALCall(alSourceStop(m_AudioSourceID));

	}

	void ALAudioSource::SetLooping(bool loop) {
		m_IsLooping = loop;
		ALCall(alSourcei(m_AudioSourceID, AL_LOOPING, m_IsLooping));
	}


	void ALAudioSource::SetGain(float gain) {
		m_Gain = gain;
		ALCall(alSourcef(m_AudioSourceID, AL_GAIN, gain));

	}

	void ALAudioSource::SetSpatial(bool spatial) {
		m_IsSpatial = spatial;
	}

	void ALAudioSource::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		ALCall(alSource3f(m_AudioSourceID, AL_POSITION, position.x, position.y, position.z));
	}


	int ALAudioSource::DeduceALFormat(int channels, int samples) {
		bool stereo = (channels > 1);

		switch (samples) {
			case 16:
				if (stereo)
					return AL_FORMAT_STEREO16;
				else
					return AL_FORMAT_MONO16;

				break;
			case 8:
				if (stereo)
					return AL_FORMAT_STEREO8;
				else
					return AL_FORMAT_MONO8;
				break;

			default:
				return 0;
		}


		RGF_ASSERT(false, "cannot deduce format!");
	}


}