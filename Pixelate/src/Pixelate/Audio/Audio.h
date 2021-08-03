#pragma once


#include "Pixelate/Audio/AudioContext.h"
#include "Pixelate/Audio/AudioListener.h"
#include "Pixelate/Audio/AudioSource.h"

#include "GLM/glm/glm.hpp"


namespace Pixelate {


	class Audio {

		public :
			static void Init();
			static void Shutdown();


			static Ref<AudioSource> CreateAudioSource(const std::string& filepath, bool shouldLoop = false, bool streaming = false);
			static void Reload(const AssetMetadata& metadata);

			static void StopAllSources(); // instead of stopping why not restart all sources?

			static void Update();
	};



}