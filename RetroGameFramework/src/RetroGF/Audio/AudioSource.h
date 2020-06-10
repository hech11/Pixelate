#pragma once

#include <string>
#include "RetroGF/Core/Core.h"
#include "GLM/glm/glm.hpp"


// Audio source contains both buffer and the source audio data. In the future I may want to seperate the buffer and the audio
// source into different classes

namespace RGF {
	
	// This may not be needed but this could be expanded to force specific arguments on the audio source
	struct AudioSourceSpecification {
		const char* filepath;
	};

	class AudioSource {
		public:
			virtual bool IsPlaying() = 0;
			virtual void Play() = 0;
			virtual void Stop() = 0;
			virtual void SetLooping(bool loop) = 0;


			virtual void SetGain(float gain) = 0;
			virtual void SetSpatial(bool spatial) = 0; // TODO: This may not need to be here.

			virtual void SetPosition(const glm::vec3& position) = 0;

			virtual unsigned int GetHandleID() = 0;
			virtual float GetPitch() = 0;
			virtual float GetGain() = 0;
			virtual bool GetSpatial() = 0; // TODO: This may not need to be here.
			virtual bool IsLooping() = 0;

		public:
			static Ref<AudioSource> Create(const AudioSourceSpecification& specs);

	};

}