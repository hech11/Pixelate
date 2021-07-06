#include "PXpch.h"
#include "EditorContentBrowser.h"

#include <imgui.h>

namespace Pixelate {

	static const std::filesystem::path s_AssetDirectory = "assets";


	EditorContentBrowser::EditorContentBrowser()
		: m_CurrentDirectory(s_AssetDirectory)
	{
	}


	void EditorContentBrowser::OnImguiRender()
	{
		ImGui::Begin("Content Browser");
		if (m_CurrentDirectory != s_AssetDirectory) {
			if (ImGui::Button("Back")) {
				m_CurrentDirectory = m_CurrentDirectory.parent_path();

			}
		}


		if (!ImGui::IsItemHovered()) {
			if (Pixelate::Input::IsMouseButtonDown(MouseButton::Right) && ImGui::IsWindowHovered())
				ImGui::OpenPopup("WindowContextPopup");

			m_CurrentContextItem = "";
		}



		for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {

			auto relPath = std::filesystem::relative(entry.path(), s_AssetDirectory);

			std::wstring wEntryPath = entry.path().wstring();
			std::string fileName = relPath.filename().string();



			if (entry.is_directory()) {
				if (ImGui::Button(fileName.c_str())) {
					m_CurrentDirectory /= entry.path().filename();
				}
			}
			else {
				if (ImGui::Button(fileName.c_str())) {
#ifdef PX_PLATFORM_WINDOWS
					// add a utils function which is platformed abstracted
					ShellExecute(NULL, 0, wEntryPath.c_str(), NULL, NULL, SW_SHOW);
#endif
				}
			}


			if (ImGui::IsItemHovered()) {

				if(Pixelate::Input::IsMouseButtonDown(MouseButton::Right))
					ImGui::OpenPopup("ContextPopup");
				m_CurrentContextItem = s_AssetDirectory / entry.path().filename();
			}

		}

		if (ImGui::BeginPopupContextItem("ContextPopup")) {
			if (ImGui::Button("Delete")) {
				m_DeleteItem = true;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Rename")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::Separator();
			if (ImGui::Button("Show file location...")) {
				ShowCurrentFileLocation();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Open file...")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();

		}
		if (ImGui::BeginPopupContextItem("WindowContextPopup")) {
			if (ImGui::Button("Create folder")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::Separator();
			if (ImGui::Button("Show file location...")) {
				ShowCurrentFileLocation();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();

		}

		ImGui::Text(m_CurrentContextItem.string().c_str());
		ImGui::End();

		if (m_DeleteItem) {
			DeleteItem();
			m_DeleteItem = false;
		}
	}

	void EditorContentBrowser::ShowCurrentFileLocation()
	{
		std::wstring args = m_CurrentDirectory.wstring();
#ifdef PX_PLATFORM_WINDOWS
		ShellExecute(NULL, L"open", args.c_str(), NULL, NULL, SW_NORMAL);
#endif
	}

	void EditorContentBrowser::RenameItem()
	{
	}

	void EditorContentBrowser::DeleteItem()
	{
		std::filesystem::remove_all(m_CurrentContextItem);
		m_CurrentContextItem = "";
	}

}