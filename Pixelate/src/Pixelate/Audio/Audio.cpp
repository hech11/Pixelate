#include "PXpch.h"
#include "Audio.h"

#include "AudioBuffer.h"


#include "Pixelate/Debug/Instrumentor.h"
#include "Pixelate/Asset/AssetManager.h"


namespace Pixelate {


	struct AudioData {
		Ref<AudioContext> Context;

		std::unordered_map<Ref<AudioBuffer>, std::vector<Ref<AudioSource>>> SourcesInScene;

		Ref<AudioMixer> OutputMixer;
		std::unordered_map<Ref<AudioMixerGroup>, std::vector<Ref<AudioSource>>> SourcesAttachedToMixers;

		bool HasInitialized = false;
	};

	static AudioData* s_Data;

	void Audio::Init() {
		PX_PROFILE_FUNCTION();

		s_Data = new AudioData;
		s_Data->Context = CreateRef<AudioContext>();

		s_Data->HasInitialized = s_Data->Context->Init();
		s_Data->OutputMixer = CreateRef<AudioMixer>();

		AudioPlatformUtils::Init();

	}
	void Audio::Shutdown() {
		PX_PROFILE_FUNCTION();
		s_Data->Context->Close();

		delete s_Data;
		AudioPlatformUtils::Shutdown();
	}


	bool Audio::HasInitializedProperly()
	{
		return s_Data->HasInitialized;
	}


	Ref<AudioSource> Audio::CreateAudioSource(const Ref<AudioBuffer>& buffer) {
		Ref<AudioSource> source = CreateRef<AudioSource>();
		source->SetBufferData(buffer);

		s_Data->SourcesInScene[buffer].emplace_back(source);

		AttachSourceToMixerGroup(source);
			
		return source;
	}

	std::unordered_map<Ref<AudioBuffer>, std::vector<Ref<AudioSource>>>& const Audio::GetAllSourcesInScene()
	{
		return s_Data->SourcesInScene;
	}

	std::unordered_map<Ref<AudioMixerGroup>, std::vector<Ref<AudioSource>>>& const Audio::GetSourcesAttachedToMixer()
	{
		return s_Data->SourcesAttachedToMixers;
	}

	Ref<AudioMixer>& Audio::GetGlobalMixer() {
		return s_Data->OutputMixer;
	}

	void Audio::StopAllSources() {
		
		for (auto& [buffer, sources] : s_Data->SourcesInScene) {
			for (auto& source : sources) {
				source->Stop();
			}
		}

	}
	

	void Audio::DestroyAllActiveSources() {
		s_Data->SourcesInScene.clear();
	}

	
	void Audio::AttachSourceToMixerGroup(const Ref<AudioSource>& source)
	{
		auto& mixer = source->GetMixerGroup();
		s_Data->SourcesAttachedToMixers[mixer].emplace_back(source);
	}

	void Audio::RemoveSourceFromMixerGroup(const Ref<AudioSource>& source) {
		int index = FindMixerGroupIndex(source);

		auto& mixer = source->GetMixerGroup();
		s_Data->SourcesAttachedToMixers[mixer].erase(index + s_Data->SourcesAttachedToMixers[mixer].begin());

	}

	int Audio::FindMixerGroupIndex(const Ref<AudioSource>& source) {
		auto& mixer = source->GetMixerGroup();
		auto& s = s_Data->SourcesAttachedToMixers[mixer];
		for (uint32_t i = 0; i < s_Data->SourcesAttachedToMixers[mixer].size(); i++) {
			if (s[i] == source) {
				return i;
				break;
			}
		}
		return -1;
	}

	void Audio::Update()
	{

// 		if (s_Data->ReloadBuffer) {
// 			s_Data->BufferToBeUpdated->SetData(s_Data->ReplacementSpec);
// 			
// 			for (int i = 0; i < s_Data->ActiveAudioSources[s_Data->BufferToBeUpdated].size(); i++) {
// 				s_Data->ActiveAudioSources[s_Data->BufferToBeUpdated][i]->SetBufferData(s_Data->BufferToBeUpdated);
// 			}
// 
// 			s_Data->ReloadBuffer = false;
// 		}
	}

	void Audio::Reload(const AssetMetadata& metadata) {

// 		std::string filepath = AssetManager::GetFilePathString(metadata);
// 
// 		if (s_Data->AudioBuffers.find(filepath) != s_Data->AudioBuffers.end()) {
// 			PX_PROFILE_SCOPE("Audio::CreateAudioSource::.AudioBuffers.find(filepath) != s_Data.AudioBuffers.end()");
// 
// 			s_Data->BufferToBeUpdated = s_Data->AudioBuffers[filepath];
// 			std::vector<Ref<AudioSource>>& allActiveSources = s_Data->ActiveAudioSources[s_Data->BufferToBeUpdated];
// 
// 			s_Data->ReplacementSpec = AudioFormatSpec::LoadAudioData(filepath);
// 			if (s_Data->ReplacementSpec.Extention == AudioFormatSpec::FileFormat::None) {
// 				PX_CORE_ERROR("Failed to reload audio buffer!\n");
// 				return;
// 			}
// 
// 			s_Data->ReloadBuffer = true;
// 			PX_CORE_MSG("updating audio buffers...\n");
// 			for (int i = 0; i < allActiveSources.size(); i++) {
// 				allActiveSources[i]->Stop();
// 			}
// 			
// 
// 		}
	}

}