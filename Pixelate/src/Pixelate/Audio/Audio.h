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


			// This would be used for queuing buffers once streaming audio is implemented
			static void Update() {}
	};

}