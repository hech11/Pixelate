#pragma once


#include "RetroGF/Audio/AudioContext.h"
#include "RetroGF/Audio/Listener.h"
#include "RetroGF/Audio/AudioSource.h"

namespace RGF {


	class Audio {

		public :
			static void Init(const AudioContext::ContextAPI& api = AudioContext::ContextAPI::OPENAL);
			static void Shutdown();

			static void PlayAudioSource(const Ref<AudioSource>& src);

			static void Update();
	};

}