#pragma once

#include "EditorPanel.h"
#include "Pixelate/Audio/AudioMixer.h"

namespace Pixelate {

	class EditorAudioMixerPanel : public EditorPanel {


		public:
			EditorAudioMixerPanel() {
				SetOpenPanel(false);
			}

			void OnImguiRender() override;
			void SetMixerContext(const Ref<AudioMixer>& mixer) { m_CurrentMixer = mixer; }

		private :
			void RenderGroupState(const std::string& id, AudioMixerStates stateId, AudioMixerStates& currentMixerState, const std::function<void(bool changedState)>& onButtonPress);
			void RenderMixer(const Ref<AudioMixer>& mixer);
			void RenderGroup(const Ref<AudioMixerGroup>& group, bool isMaster = false);
		private :
			Ref<AudioMixer> m_CurrentMixer;

	};

}