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

			static bool HasInitializedProperly();


			static Ref<AudioSource> CreateAudioSource(const Ref<AudioBuffer>& buffer);


			static void Reload(const AssetMetadata& metadata);

			static void StopAllSources();
			static void DestroyAllActiveSources();


			static void Update();
		
	};



}