#include "PXpch.h"
#include "Pixelate/Audio/AudioBuffer.h"
#include <Pixelate\Platform\OpenAL\ALCommon.h>

#include "Pixelate/Debug/Instrumentor.h"
namespace Pixelate {


	int DeduceALFormat(int channels) {
		PX_PROFILE_FUNCTION();
		bool stereo = (channels > 1);
		if (stereo) return AL_FORMAT_STEREO16;

		return AL_FORMAT_MONO16;
	}


	AudioBuffer::AudioBuffer() {
		PX_PROFILE_FUNCTION();
		ALCall(alGenBuffers(1, &m_AudioID));
	}


	AudioBuffer::AudioBuffer(const AudioFileSpecification& specs) {
		PX_PROFILE_FUNCTION();
		ALCall(alGenBuffers(1, &m_AudioID));
		SetData(specs);
	}


	AudioBuffer::~AudioBuffer() {
		PX_PROFILE_FUNCTION();
		ALCall(alDeleteBuffers(1, &m_AudioID));
	}

	void AudioBuffer::SetData(const AudioFileSpecification& specs) {

		PX_PROFILE_FUNCTION();
		ALCall(alBufferData(m_AudioID, DeduceALFormat(specs.Channels),
			specs.Data, specs.Size, specs.SampleRate));
	}

}