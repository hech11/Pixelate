#include "RGFpch.h"
#include "AudioSource.h"

#include "AudioContext.h"

#include "RetroGF/Platform/OpenAL/ALAudioSource.h"

namespace RGF {

	Ref<AudioSource> AudioSource::Create(const AudioSourceSpecification& specs) {
		switch (AudioContext::GetContext()) {
			case AudioContext::ContextAPI::OPENAL:
				return CreateRef<ALAudioSource>(specs);
		}
	}


}