#include "PXpch.h"
#include "EditorAudioMixerPanel.h"

#include "Pixelate/Audio/Audio.h"

#include <imgui.h>


namespace Pixelate {

	void EditorAudioMixerPanel::RenderGroupState(const std::string& id, AudioMixerStates stateId, AudioMixerStates& currentMixerState) {
		bool pushCol = false;
		if (currentMixerState == stateId) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.11f, 0.11f, 0.11f, 1.0f });
			pushCol = true;
		}
		if (ImGui::Button(id.c_str())) {
			if (currentMixerState == stateId)
				currentMixerState = AudioMixerStates::Default;
			else
				currentMixerState = stateId;
		}
		if (pushCol)
			ImGui::PopStyleColor();

	}


	void EditorAudioMixerPanel::RenderGroup(const Ref<AudioMixerGroup>& group) {
		std::stringstream imguiID;
		imguiID << "##" << group->DebugName;
		ImGui::BeginChild(imguiID.str().c_str(), {150.0f, 450.0f}, true);
		ImGui::Text(group->DebugName.c_str());

		float peak = AudioPlatformUtils::GetPeakValue();
		float pResult = group->Gain * peak; // TODO: not quite VU... 

		const float speakerVUValues[] = { pResult, pResult };

		ImGui::PlotHistogram("##volume", speakerVUValues, IM_ARRAYSIZE(speakerVUValues), 0, NULL, 0.0f, 0.5f, ImVec2(28, 256));
		ImGui::SameLine();
		ImGui::VSliderFloat("##gainController", ImVec2(28, 256), &group->Gain, 0.0f, 2.0f, "");
		ImGui::Separator();



		ImGui::Text(std::to_string(group->Gain).c_str());

		AudioMixerStates& state = group->State;

		RenderGroupState("Solo", AudioMixerStates::Solo, state);
		ImGui::SameLine();
		RenderGroupState("Mute", AudioMixerStates::Mute, state);
		ImGui::SameLine();
		RenderGroupState("Bypass", AudioMixerStates::Bypass, state);


		ImGui::EndChild();

	}


	void EditorAudioMixerPanel::OnImguiRender() {
		ImGui::Begin("Audio Mixer", &m_IsPanelOpen);

		ImGui::Columns(2);
		ImGui::SetColumnOffset(1, 200.0f);

		ImGui::Text("hierarchy here");
		if (ImGui::Button("+")) {
			Ref<AudioMixerGroup> group = CreateRef<AudioMixerGroup>();
			group->DebugName = "Group" + std::to_string(m_CurrentMixer->GetGroups().size() + 1);
			m_CurrentMixer->GetGroups().push_back(group);
		}
		ImGui::NextColumn();


		RenderGroup(m_CurrentMixer->GetMasterGroup());
		ImGui::SameLine();
		for (auto& group : m_CurrentMixer->GetGroups()) {
			RenderGroup(group);
			ImGui::SameLine();
		}


		ImGui::Columns(1);
		ImGui::End();

	}

}