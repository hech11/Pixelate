#include "PXpch.h"
#include "Audio.h"

#include "AudioBuffer.h"


#include "Pixelate/Debug/Instrumentor.h"

namespace Pixelate {


	struct AudioData {
		Ref<AudioContext> Context;

		std::unordered_map<std::string, Ref<AudioBuffer>> AudioBuffers;
	};

	static AudioData* s_Data;

	void Audio::Init() {
		PX_PROFILE_FUNCTION();

		s_Data = new AudioData;
		s_Data->Context = CreateRef<AudioContext>();
		s_Data->Context->Init();



	}
	void Audio::Shutdown() {
		PX_PROFILE_FUNCTION();
		s_Data->Context->Close();

		delete s_Data;
	}


	Ref<AudioSource> Audio::CreateAudioSource(const std::string& filepath, bool shouldLoop /*= false*/, bool streaming /*= false*/) {
		PX_PROFILE_FUNCTION();

		Ref<AudioSource> source = CreateRef<AudioSource>();
		source->SetLooping(shouldLoop);

		if (s_Data->AudioBuffers.find(filepath) != s_Data->AudioBuffers.end()) {
			PX_PROFILE_SCOPE("Audio::CreateAudioSource::.AudioBuffers.find(filepath) != s_Data.AudioBuffers.end()");
			source->SetBufferData(s_Data->AudioBuffers[filepath]);
			return source;
		}

		AudioFormatSpec specs = AudioFormatSpec::LoadAudioData(filepath);
		if (specs.Extention == AudioFormatSpec::FileFormat::None) {
			return nullptr;
		}
		Ref<AudioBuffer> buffer = CreateRef<AudioBuffer>(specs);

		s_Data->AudioBuffers[filepath] = buffer;
		source->SetBufferData(buffer);

		return source;
	}

	
	

}