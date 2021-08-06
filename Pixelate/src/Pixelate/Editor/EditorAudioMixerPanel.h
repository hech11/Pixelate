#pragma once

#include "EditorPanel.h"
#include "Pixelate/Audio/AudioMixer.h"

namespace Pixelate {

	class EditorAudioMixerPanel : public EditorPanel {


		public:
			void OnImguiRender() override;
			void SetMixerContext(const Ref<AudioMixer>& mixer) { m_CurrentMixer = mixer; }

		private :
			void RenderGroupState(const std::string& id, AudioMixerStates stateId, AudioMixerStates& currentMixerState);
			void RenderGroup(const Ref<AudioMixerGroup>& group);
		private :
			Ref<AudioMixer> m_CurrentMixer;

	};

}