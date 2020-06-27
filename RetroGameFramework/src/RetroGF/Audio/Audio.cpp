#include "RGFpch.h"
#include "Audio.h"

#include "AudioBuffer.h"

namespace RGF {


	struct AudioData {
		Ref<AudioContext> Context;
		Ref<AudioListener> Listener;

		std::map<std::string, Ref<AudioBuffer>> AudioBuffers;
	};

	static AudioData s_Data;

	void Audio::Init() {

		s_Data.Context = CreateRef<AudioContext>();
		s_Data.Context->Init();

		s_Data.Listener = CreateRef<AudioListener>();


	}
	void Audio::Shutdown() {
		s_Data.Context->Close();
	}


	Ref<AudioSource> Audio::CreateAudioSource(const std::string& filepath, bool shouldLoop /*= false*/, bool streaming /*= false*/) {

		Ref<AudioSource> source = CreateRef<AudioSource>();
		source->SetLooping(shouldLoop);

		if (s_Data.AudioBuffers.find(filepath) != s_Data.AudioBuffers.end()) {

			source->SetBufferData(s_Data.AudioBuffers[filepath]);
			return source;
		}

		AudioFormatSpec specs = AudioFormatSpec::LoadAudioData(filepath);
		Ref<AudioBuffer> buffer = CreateRef<AudioBuffer>(specs);

		s_Data.AudioBuffers[filepath] = buffer;
		source->SetBufferData(buffer);

		return source;
	}

	
	

}