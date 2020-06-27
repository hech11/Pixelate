#pragma once


#include "RetroGF/Audio/AudioContext.h"
#include "RetroGF/Audio/AudioListener.h"
#include "RetroGF/Audio/AudioSource.h"

#include "GLM/glm/glm.hpp"

namespace RGF {


	class Audio {

		public :
			static void Init();
			static void Shutdown();


			static Ref<AudioSource> CreateAudioSource(const std::string& filepath, bool shouldLoop = false, bool streaming = false);


			// This would be used for queuing buffers once streaming audio is implemented
			static void Update() {}
	};

}