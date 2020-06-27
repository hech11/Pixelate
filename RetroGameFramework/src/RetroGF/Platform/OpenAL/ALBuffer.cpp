#include "RGFpch.h"
#include "RetroGF/Audio/AudioBuffer.h"
#include <RetroGF\Platform\OpenAL\ALCommon.h>

namespace RGF {


	int DeduceALFormat(int channels) {
		bool stereo = (channels > 1);
		if (stereo) return AL_FORMAT_STEREO16;

		return AL_FORMAT_MONO16;
	}


	AudioBuffer::AudioBuffer(const AudioFormatSpec& specs) {
		ALCall(alGenBuffers(1, &m_AudioID));
		SetData(specs);
	}


	AudioBuffer::~AudioBuffer() {
		ALCall(alDeleteBuffers(1, &m_AudioID));
	}

	void AudioBuffer::SetData(const AudioFormatSpec& specs) {

		ALCall(alBufferData(m_AudioID, DeduceALFormat(specs.Channels),
			specs.Data, specs.Size, specs.SampleRate));
	}

}