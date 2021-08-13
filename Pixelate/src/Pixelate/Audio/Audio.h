#pragma once


#include "Pixelate/Audio/AudioContext.h"
#include "Pixelate/Audio/AudioListener.h"
#include "Pixelate/Audio/AudioPlatformUtils.h"
#include "Pixelate/Audio/AudioMixer.h"
#include "Pixelate/Audio/AudioSource.h"

#include "GLM/glm/glm.hpp"


namespace Pixelate {


	class Audio {

		public :


			static void Init();
			static void Shutdown();

			static bool HasInitializedProperly();

			static Ref<AudioSource> CreateAudioSource(const Ref<AudioBuffer>& buffer);
			static std::unordered_map< Ref<AudioBuffer>, std::vector<Ref<AudioSource>>>& const GetAllSourcesInScene();

			static std::unordered_map<Ref<AudioMixerGroup>, std::vector<Ref<AudioSource>>>& const GetSourcesAttachedToMixer();

			static const Ref<AudioMixer>& const GetGlobalMixer();
			static void SetGlobalMixer(const Ref<AudioMixer>& mixer);

			static void Reload(const AssetMetadata& metadata);

			static void StopAllSources();
			static void DestroyAllActiveSources();

			static void AttachSourceToMixerGroup(const Ref<AudioSource>& source);
			static void RemoveSourceFromMixerGroup(const Ref<AudioSource>& source);

			static void UpdateMixerSourceGain(const Ref<AudioMixerGroup>& group);
			static void SoloAudioGroup(const Ref<AudioMixerGroup>& group);
			static void UnsoloAudioGroup(const Ref<AudioMixerGroup>& group);

			static void MuteAudioGroup(const Ref<AudioMixerGroup>& group, bool mute = true);


			// Returns index of the sources attached to the mixer group. Returns -1 if we cannot find the source index.
			static int FindMixerGroupIndex(const Ref<AudioSource>& source);




			static void Update();

		
	};



}