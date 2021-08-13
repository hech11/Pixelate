#include "PXpch.h"
#include "EditorAudioPanel.h"


#include <imgui.h>
#include "Pixelate/Audio/Audio.h"
#include "Pixelate/Asset/AssetManager.h"
#include "Pixelate/Audio/AudioMixerImporter.h"

#include "../../Pixelate-Editor/vendor/NativeFileDialog/src/include/nfd.h"
#include "EditorAudioMixerPanel.h"
#include "EditorPanel.h"


namespace Pixelate {


	void EditorAudioPanel::OnImguiRender()
	{
		if (!m_IsPanelOpen)
			return;


		ImGui::Begin("Audio", &m_IsPanelOpen);

		ImGui::Text("Audio Mixer:");
		ImGui::SameLine();
		char temp[50];
		memset(temp, 0, 50);

		// TODO: Asset importer
		if (ImGui::Button("Import...")) {

			nfdchar_t* outPath = NULL;
			nfdresult_t result = NFD_OpenDialog("ampx", NULL, &outPath);

			if (result == NFD_OKAY) {
				const auto& mixer = AudioMixerImporter::Import(outPath);
				Audio::SetGlobalMixer(mixer);

				const auto& panel = std::dynamic_pointer_cast<EditorAudioMixerPanel>(EditorPanelManager::Get().GetPanel("AudioMixerPanel"));
				panel->SetMixerContext(mixer);
			}
			free(outPath);

		}
		ImGui::SameLine();
		if (ImGui::Button("Export...")) {
// 			nfdchar_t* outPath = NULL;
// 			nfdresult_t result = NFD_SaveDialog("ampx", NULL, &outPath);
// 
// 			if (result == NFD_OKAY) {
// 				AudioMixerImporter::Export(outPath, Audio::GetGlobalMixer());
// 			}
// 			free(outPath);
		}
		ImGui::SameLine();
		ImGui::InputText("##AudioMixerPath", temp, 50, ImGuiInputTextFlags_ReadOnly);

		ImGui::Separator();
		auto& sourcesInScene = Audio::GetAllSourcesInScene();


		ImGui::Text("Audio details in scene");
		ImGui::Separator();
		ImGui::Columns(2);
		ImGui::Text("Buffers");
		ImGui::NextColumn();
		ImGui::Text("Sources");
		ImGui::Columns(1);
		ImGui::Separator();

		ImGui::Columns(2);
		for (auto&[buffer, sources] : sourcesInScene)
		{
			ImGui::Text("Handle: %s", std::to_string(buffer->Handle).c_str());
			ImGui::Text("Filepath: %s", AssetManager::GetFilePathString(AssetManager::GetMetadata(buffer->Handle)).c_str());
			ImGui::NextColumn();

			for (auto& a : m_SceneContext->GetAllEntitiesWith<AudioSourceComponent>()) {

				Entity e = { a, m_SceneContext.get() };


				for (auto& source : sources) {
					if (e.GetComponent<AudioSourceComponent>().Source == source) {
						std::string name = e.GetComponent<NameComponent>().Name;
						UUID uuid = e.GetComponent<UUIDComponent>().UUID;


						ImGui::Text("Entity name: %s", name.c_str());
						ImGui::TextDisabled(std::to_string(uuid).c_str());
						break;
					}
				}

			}
			ImGui::NextColumn();
			ImGui::Separator();

		}
		ImGui::Columns(1);


		ImGui::End();
	}




}

