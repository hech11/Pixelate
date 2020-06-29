#include "RGFpch.h"
#include "RetroGF/Audio/AudioBuffer.h"
#include <RetroGF\Platform\OpenAL\ALCommon.h>

#include "RetroGF/Debug/Instrumentor.h"
namespace RGF {


	int DeduceALFormat(int channels) {
		RGF_PROFILE_FUNCTION();
		bool stereo = (channels > 1);
		if (stereo) return AL_FORMAT_STEREO16;

		return AL_FORMAT_MONO16;
	}


	AudioBuffer::AudioBuffer(const AudioFormatSpec& specs) {
		RGF_PROFILE_FUNCTION();
		ALCall(alGenBuffers(1, &m_AudioID));
		SetData(specs);
	}


	AudioBuffer::~AudioBuffer() {
		RGF_PROFILE_FUNCTION();
		ALCall(alDeleteBuffers(1, &m_AudioID));
	}

	void AudioBuffer::SetData(const AudioFormatSpec& specs) {

		RGF_PROFILE_FUNCTION();
		ALCall(alBufferData(m_AudioID, DeduceALFormat(specs.Channels),
			specs.Data, specs.Size, specs.SampleRate));
	}

}