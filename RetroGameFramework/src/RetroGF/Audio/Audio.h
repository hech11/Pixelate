#pragma once


#include "RetroGF/Audio/AudioContext.h"

namespace RGF {


	class Audio {

		public :
			static void Init(const AudioContext::ContextAPI& api = AudioContext::ContextAPI::OPENAL);
			static void Shutdown();


		private :
			static AudioContext* m_Context;
	};

}