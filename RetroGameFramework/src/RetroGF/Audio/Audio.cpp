#include "RGFpch.h"
#include "Audio.h"


namespace RGF {


	struct AudioData {
		AudioContext* m_Context;
		Ref<Listener> m_Listener;
	};

	static AudioData m_Data;

	void Audio::Init(const AudioContext::ContextAPI& api) {

		m_Data.m_Context = AudioContext::CreateContext(api);
		m_Data.m_Context->Init();

		m_Data.m_Listener = Listener::Create();

	}
	void Audio::Shutdown() {
		m_Data.m_Context->Close();
	}

	void Audio::PlayAudioSource(const Ref<AudioSource>& src) {
		src->Play();
	}

	void Audio::Update() {

	}


}