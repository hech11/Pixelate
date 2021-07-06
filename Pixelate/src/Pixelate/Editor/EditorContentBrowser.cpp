#include "PXpch.h"
#include "EditorContentBrowser.h"

#include <imgui.h>

namespace Pixelate {


	// TODO: This needs to change when introducing different asset dir paths, projects and a asset manager.
	static const std::filesystem::path s_AssetDirectory = "assets";


	EditorContentBrowser::EditorContentBrowser()
		: m_CurrentDirectory(s_AssetDirectory)
	{
	}

	static void RenderDirectory(const std::filesystem::directory_entry& dir) {


		std::string id = dir.path().string();

		if (dir.is_directory()) {
			bool open = ImGui::TreeNode(id.c_str());
			if (open) {
				ImGui::TreePop();
			}

		}
	}

	void EditorContentBrowser::RenderTopBar() {

		ImGui::BeginChild("##topbar", ImVec2(0, 30));
		{
			if (m_CurrentDirectory != s_AssetDirectory) {
				if (ImGui::Button("Back")) {
					m_CurrentDirectory = m_CurrentDirectory.parent_path();

				}
			}

			ImGui::EndChild();

		}


	}

	void EditorContentBrowser::OnImguiRender()
	{
		ImGui::Begin("Content Browser");
		ImGui::Columns(2);
		ImGui::SetColumnOffset(1, 300.0f);
		ImGui::BeginChild("##folders");
		{
			if (ImGui::CollapsingHeader("Content", 0, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen)) {
		

				for (auto& entry : std::filesystem::directory_iterator(s_AssetDirectory)) {
					RenderDirectory(entry);
				
				}
			}


			ImGui::EndChild();
		}



		ImGui::NextColumn();

		ImGui::BeginChild("##dir", ImVec2(0, ImGui::GetWindowHeight() - 65));
		{
			RenderTopBar();
			ImGui::Separator();


			if (!ImGui::IsItemHovered()) {
				if (Pixelate::Input::IsMouseButtonDown(MouseButton::Right) && ImGui::IsWindowHovered())
					ImGui::OpenPopup("WindowContextPopup");

			}

			int entryIterator = 0;

			for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {

				auto relPath = std::filesystem::relative(entry.path(), s_AssetDirectory);

				std::wstring wEntryPath = entry.path().wstring();
				std::string fileName = relPath.filename().string();
				std::string displayName = "##" + fileName;
				std::string childName = fileName + fileName;


				if ((entryIterator % 4) != 0)
					ImGui::SameLine(entryIterator * ImGui::GetContentRegionAvail().x / 4.0f);


				if (entry.is_directory()) {
					if (ImGui::Button(fileName.c_str())) {
						m_CurrentDirectory /= entry.path().filename();
					}
				} else {
					if (ImGui::Button(fileName.c_str())) {
#ifdef PX_PLATFORM_WINDOWS
						// add a utils function which is platformed abstracted
						ShellExecute(NULL, 0, wEntryPath.c_str(), NULL, NULL, SW_SHOW);
#endif
					}

				}
				
				if (ImGui::IsItemHovered()) {

					if (Pixelate::Input::IsMouseButtonDown(MouseButton::Right)) {
						ImGui::OpenPopup("ContextPopup");
						m_CurrentContextItem = s_AssetDirectory / entry.path().filename();
					}
				}
				ImGui::NextColumn();
				entryIterator++;
			}
			ImGui::Text(m_CurrentContextItem.string().c_str());
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


			ImGui::EndChild();
			ImGui::Columns(1);
			ImGui::Text(m_CurrentContextItem.string().c_str());
			ImGui::End();

			if (m_DeleteItem) {
				DeleteItem();
				m_DeleteItem = false;
			}
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