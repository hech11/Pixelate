#include "PXpch.h"
#include "Audio.h"

#include "AudioBuffer.h"


#include "Pixelate/Debug/Instrumentor.h"
#include "Pixelate/Asset/AssetManager.h"

namespace Pixelate {


	struct AudioData {
		Ref<AudioContext> Context;
		std::unordered_map<std::string, Ref<AudioBuffer>> AudioBuffers;
		std::unordered_map<Ref<AudioBuffer>, std::vector<Ref<AudioSource>>> ActiveAudioSources;

		bool ReloadBuffer = false;
		AudioFormatSpec ReplacementSpec;
		Ref<AudioBuffer> BufferToBeUpdated;
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
			Ref<AudioBuffer> buffer = s_Data->AudioBuffers[filepath];

			s_Data->ActiveAudioSources[buffer].push_back(source);
			source->SetBufferData(buffer);

			return source;
		}

		AudioFormatSpec specs = AudioFormatSpec::LoadAudioData(filepath);
		if (specs.Extention == AudioFormatSpec::FileFormat::None) {
			return nullptr;
		}
		Ref<AudioBuffer> buffer = CreateRef<AudioBuffer>(specs);

		s_Data->AudioBuffers[filepath] = buffer;
		s_Data->ActiveAudioSources[buffer].push_back(source);

		source->SetBufferData(buffer);

		return source;
	}

	void Audio::StopAllSources() {
		for (auto& [buffer, source] : s_Data->ActiveAudioSources) {
			for (int i = 0; i < source.size(); i++) {
				Ref<AudioSource>& s = source[i];
				if (s->IsPlaying()) {
					s->Stop();
				}
			}
		}
	}
	

	void Audio::Update()
	{

		if (s_Data->ReloadBuffer) {
			s_Data->BufferToBeUpdated->SetData(s_Data->ReplacementSpec);
			
			for (int i = 0; i < s_Data->ActiveAudioSources[s_Data->BufferToBeUpdated].size(); i++) {
				s_Data->ActiveAudioSources[s_Data->BufferToBeUpdated][i]->SetBufferData(s_Data->BufferToBeUpdated);
			}

			s_Data->ReloadBuffer = false;
		}
	}

	void Audio::Reload(const AssetMetadata& metadata) {

		std::string filepath = AssetManager::GetFilePathString(metadata);

		if (s_Data->AudioBuffers.find(filepath) != s_Data->AudioBuffers.end()) {
			PX_PROFILE_SCOPE("Audio::CreateAudioSource::.AudioBuffers.find(filepath) != s_Data.AudioBuffers.end()");

			s_Data->BufferToBeUpdated = s_Data->AudioBuffers[filepath];
			std::vector<Ref<AudioSource>>& allActiveSources = s_Data->ActiveAudioSources[s_Data->BufferToBeUpdated];

			s_Data->ReplacementSpec = AudioFormatSpec::LoadAudioData(filepath);
			if (s_Data->ReplacementSpec.Extention == AudioFormatSpec::FileFormat::None) {
				PX_CORE_ERROR("Failed to reload audio buffer!\n");
				return;
			}

			s_Data->ReloadBuffer = true;
			PX_CORE_MSG("updating audio buffers...\n");
			for (int i = 0; i < allActiveSources.size(); i++) {
				allActiveSources[i]->Stop();
			}
			

		}
	}

}