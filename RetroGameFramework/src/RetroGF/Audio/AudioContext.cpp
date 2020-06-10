#include "RGFpch.h"
#include "AudioContext.h"


#include "RetroGF/Platform/OpenAL/ALAudioContext.h"

namespace RGF {



	AudioContext::ContextAPI AudioContext::m_Context = AudioContext::ContextAPI::None;

	AudioContext* AudioContext::CreateContext(const AudioContext::ContextAPI& context) {
		m_Context = context;
		switch (context) {
		case AudioContext::ContextAPI::OPENAL:
			return new ALAudioContext;
		}
	}

}