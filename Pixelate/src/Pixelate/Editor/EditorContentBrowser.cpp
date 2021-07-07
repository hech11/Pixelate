#include "PXpch.h"
#include "EditorContentBrowser.h"

#include <imgui.h>
#include "Pixelate/Utility/FileSystem.h"

namespace Pixelate {


	// TODO: This needs to change when introducing different asset dir paths, projects and a asset manager.
	static const std::filesystem::path s_AssetDirectory = "assets";

	static const std::string s_DefaultItemName = "New item"; // This should be changed when creating an asset manager.
	static const std::string s_DefaultFolderName = "New folder"; // This should be changed when creating an asset manager.


	EditorContentBrowser::EditorContentBrowser()
		: m_CurrentDirectory(s_AssetDirectory)
	{
	}

	void EditorContentBrowser::RenderDirectory(const std::filesystem::path& dir) {


		auto relPath = std::filesystem::relative(dir, s_AssetDirectory);

		if (FileSystem::IsDirectory(dir)) {
			bool open = ImGui::TreeNodeEx(dir.string().c_str(), ImGuiTreeNodeFlags_OpenOnArrow);
			if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0)) {
				m_CurrentDirectory = s_AssetDirectory / dir.filename();
			}
			if (open) {

				for (auto& entry : std::filesystem::directory_iterator(dir)) {
					auto currentPath = entry.path();
					RenderDirectory(currentPath);
				}

				ImGui::TreePop();
			}
		}
		else {
			ImGui::Button(dir.string().c_str());
			if (Input::IsMouseButtonDown(MouseButton::Right) && ImGui::IsItemHovered() && m_IfHoveredOverItem) {
				ImGui::OpenPopup("ContextPopup");

				m_OpenContextPopup = true;
				m_CurrentContextItem = s_AssetDirectory / relPath;
				m_CurrentFileName = dir.filename().string();

			}

			if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0)) {
				FileSystem::OpenFileExternally(dir);
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

	void EditorContentBrowser::DrawItemContext()
	{
		if (ImGui::BeginPopupContextItem("ContextPopup")) {
			if (ImGui::Button("Copy")) {
				m_CurrentCopiedItem = m_CurrentContextItem;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Paste")) {
				if (m_CurrentCopiedItem != "") {
					CopyItem();
				}
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Delete")) {
				m_DeleteItem = true;
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Rename")) {
				m_RenameMode = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::Separator();
			if (ImGui::Button("Open file...")) {
				if (!FileSystem::IsDirectory(m_CurrentContextItem))
					FileSystem::OpenFileExternally(m_CurrentContextItem);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Show file location...")) {
				ShowCurrentFileLocation();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();

		}
	}

	void EditorContentBrowser::DrawWindowContext()
	{
		if (ImGui::BeginPopupContextItem("WindowContextPopup")) {
			if (ImGui::Button("Paste")) {
				if (m_CurrentCopiedItem != "") {
					CopyItem();
				}
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Create folder")) {
				CreateFolder();
				ImGui::CloseCurrentPopup();
			}
			ImGui::Separator();
			if (ImGui::Button("Show file location...")) {
				ShowCurrentFileLocation();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();

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
					RenderDirectory(entry.path());
				}
			}



			ImGui::EndChild();
		}

		ImGui::NextColumn();

		ImGui::BeginChild("##dir", ImVec2(0, ImGui::GetWindowHeight() - 65));
		{
			RenderTopBar();
			ImGui::Separator();


			if (!m_IfHoveredOverItem) {
				if (Pixelate::Input::IsMouseButtonDown(MouseButton::Right) && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
					m_OpenWindowContextPopup = true;
			}

			ImGui::Columns(4, 0, false);
			for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {

				auto relPath = std::filesystem::relative(entry.path(), s_AssetDirectory);
				std::string fileName = relPath.filename().string();

				if (!m_RenameMode) {
					if (entry.is_directory()) {
						if (ImGui::Button(fileName.c_str())) {
							m_CurrentDirectory /= entry.path().filename();
						}

					} else {
						if(ImGui::Button(fileName.c_str())){
							FileSystem::OpenFileExternally(entry.path());
						}
					}
				} else {
					if (m_CurrentFileName == fileName)
					{
						char buffer[255];
						memset(buffer, 0, 255);
						memcpy(buffer, m_CurrentFileName.c_str(), m_CurrentFileName.length());
						std::string temp = "##" + m_CurrentFileName;
						ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
						ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
						ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
						ImGui::InputText(temp.c_str(), buffer, 255);


						if (Input::IsKeyDown(KeyCode::Enter) || (Input::IsMouseButtonDown(MouseButton::Left) && !ImGui::IsItemHovered())) {
							RenameItem(buffer);
							m_RenameMode = false;
						}


						ImGui::PopStyleColor(3);
					} else {
						if (entry.is_directory()) {
							if (ImGui::Button(fileName.c_str())) {
								m_CurrentDirectory /= entry.path().filename();
							}
						}
						else {
							if(ImGui::Button(fileName.c_str())){
								FileSystem::OpenFileExternally(entry.path());
							}

						}
					}
				}


				
				if (ImGui::IsItemHovered() && m_IfHoveredOverItem) {

					if (Pixelate::Input::IsMouseButtonDown(MouseButton::Right)) {
						m_CurrentContextItem = s_AssetDirectory / relPath;
						m_CurrentFileName = fileName;

						m_OpenContextPopup = true;
					}
				}
				ImGui::NextColumn();
			}
			ImGui::Text(m_CurrentContextItem.string().c_str());
			

			if (m_OpenContextPopup) {
				ImGui::OpenPopup("ContextPopup");
				m_OpenContextPopup = false;
			}
			if (m_OpenWindowContextPopup) {
				ImGui::OpenPopup("WindowContextPopup");
				m_OpenWindowContextPopup = false;
			}

			DrawItemContext();
			DrawWindowContext();


			ImGui::EndChild();
			ImGui::Columns(1);


			if (m_DeleteItem) {
				DeleteItem();
				m_DeleteItem = false;
			}

			if (ImGui::IsAnyItemHovered()) {
				m_IfHoveredOverItem = true;
			}
			else {
				m_IfHoveredOverItem = false;
			}


			std::stringstream hovered;
			hovered << "is hovered: " << (m_IfHoveredOverItem ? "true" : "false");
			ImGui::Text(hovered.str().c_str());
			ImGui::End();


		}

		

	}

	void EditorContentBrowser::ShowCurrentFileLocation()
	{
		FileSystem::ShowFileInExplorer(m_CurrentDirectory);
	}

	void EditorContentBrowser::RenameItem(const std::string& newName)
	{
		FileSystem::RenameDirectory(m_CurrentContextItem, newName);
		m_CurrentFileName = "";
		m_CurrentContextItem = "";
	}

	void EditorContentBrowser::DeleteItem()
	{
		FileSystem::DeleteDirectory(m_CurrentContextItem.string());
		m_CurrentContextItem = "";
		m_CurrentFileName = "";

	}

	void EditorContentBrowser::CopyItem() {
		std::filesystem::path newFile = m_CurrentCopiedItem;
		std::stringstream extention;

		bool alreadyACopy = false;

		for (int i = 1; FileSystem::Exists(newFile); i++) {
			extention.str(std::string());
			if(alreadyACopy)
				extention << " - Copy" << " (" << i << ")";
			else
				extention << " - Copy";


			newFile = (m_CurrentCopiedItem.string() + extention.str());
			alreadyACopy = true;
		}


		FileSystem::CopyDirectory(m_CurrentCopiedItem, newFile);
	}

	void EditorContentBrowser::CreateFolder()
	{
		std::filesystem::path newFile = m_CurrentDirectory;
		std::stringstream extention;
	    newFile /= s_DefaultFolderName;

		for (int i = 2; FileSystem::Exists(newFile); i++) {
			extention.str(std::string());
			extention << " (" << i << ")";

			newFile = m_CurrentDirectory / (s_DefaultFolderName + extention.str());
		}


		FileSystem::CreateDirectory(newFile);
	}

}