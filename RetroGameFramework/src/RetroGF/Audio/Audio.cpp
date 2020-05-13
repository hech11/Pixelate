#include "RGFpch.h"
#include "Audio.h"


namespace RGF {

	AudioContext* Audio::m_Context = nullptr;

	void Audio::Init(const AudioContext::ContextAPI& api) {
		m_Context = AudioContext::CreateContext(api);
		m_Context->Init();
	}
	void Audio::Shutdown() {
		m_Context->Close();

	}

}