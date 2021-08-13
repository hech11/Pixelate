#include "PXpch.h"
#include "EditorAudioMixerPanel.h"

#include "Pixelate/Audio/Audio.h"

#include <imgui.h>
#include <type_traits>


namespace Pixelate {

	void EditorAudioMixerPanel::RenderGroupState(const std::string& id, AudioMixerStates stateId, AudioMixerStates& currentMixerState, const std::function<void(bool changedState)>& onButtonPress) {
		bool pushCol = false;
		if (currentMixerState == stateId) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.11f, 0.11f, 0.11f, 1.0f });
			pushCol = true;
		}
		if (ImGui::Button(id.c_str())) {
			bool changed = false;
			if (currentMixerState == stateId)
				currentMixerState = AudioMixerStates::Default;
			else {
				currentMixerState = stateId;
				changed = true;
			}
			onButtonPress(changed);
		}
		if (pushCol)
			ImGui::PopStyleColor();

	}


	void EditorAudioMixerPanel::RenderMixer(const Ref<AudioMixer>& mixer)
	{
		RenderGroup(mixer->GetMasterGroup(), true);
		ImGui::SameLine();

		for (auto& group : mixer->GetGroups()) {
			RenderGroup(group);
			ImGui::SameLine();
		}

	}

	void EditorAudioMixerPanel::RenderGroup(const Ref<AudioMixerGroup>& group, bool isMaster) {

		size_t hash = std::hash<std::string>{}(group->DebugName); // cache these?
		ImGui::BeginChild(std::string(group->DebugName).c_str(), {150.0f, 450.0f}, true);

		ImGui::Text(group->DebugName.c_str());

		float peak = 0.0f;
		if (isMaster)
			peak = AudioPlatformUtils::GetPeakValue();
		else {

		}


		float pResult = group->Gain * peak;

		const float speakerVUValues[] = { pResult, pResult };

		ImGui::PlotHistogram("##volume", speakerVUValues, IM_ARRAYSIZE(speakerVUValues), 0, NULL, 0.0f, 0.5f, ImVec2(28, 256));
		ImGui::SameLine();
		if (ImGui::VSliderFloat("##gainController", ImVec2(28, 256), &group->Gain, 0.0f, 2.0f, "")) {

			Audio::UpdateMixerSourceGain(group);
		}
		ImGui::Separator();



		ImGui::Text(std::to_string(group->Gain).c_str());

		AudioMixerStates& state = group->State;

		RenderGroupState("Solo", AudioMixerStates::Solo, state, [&](bool changed) {
			if (changed) {
				Audio::SoloAudioGroup(group);
			} else {
				Audio::UnsoloAudioGroup(group);
			}
		});
		ImGui::SameLine();
		RenderGroupState("Mute", AudioMixerStates::Mute, state, [&](bool changed) {
			Audio::MuteAudioGroup(group, changed);
		});
		ImGui::SameLine();
		RenderGroupState("Bypass", AudioMixerStates::Bypass, state, [&](bool changed) {
			PX_CORE_WARN("Bypass not implemented!\n");
		});


		ImGui::EndChild();

	}


	void EditorAudioMixerPanel::OnImguiRender() {
		if (!m_IsPanelOpen)
			return;

		ImGui::Begin("Audio Mixer WIP", &m_IsPanelOpen);

		ImGui::Columns(2);
		ImGui::SetColumnOffset(1, 200.0f);

		ImGui::Text("hierarchy here");
		if (ImGui::Button("+")) {
			Ref<AudioMixerGroup> group = CreateRef<AudioMixerGroup>();
			group->DebugName = "Group" + std::to_string(m_CurrentMixer->GetGroups().size() + 1);
			m_CurrentMixer->GetGroups().push_back(group);
		}
		ImGui::NextColumn();

		RenderMixer(m_CurrentMixer);


		ImGui::Columns(1);
		ImGui::End();

	}

}