#include "PXpch.h"
#include "EditorContentBrowser.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "Pixelate/Utility/FileSystem.h"

namespace Pixelate {


	// TODO: This needs to change when introducing different asset dir paths, projects and a asset manager.
	static const std::filesystem::path s_AssetDirectory = "assets";

	static const std::string s_DefaultItemName = "New item"; // This should be changed when creating an asset manager.
	static const std::string s_DefaultFolderName = "New folder"; // This should be changed when creating an asset manager.


	EditorContentBrowser::EditorContentBrowser()
		: m_CurrentDirectory(s_AssetDirectory)
	{
		m_FolderIcon = Texture::Create("resources/icons/content-browser/folder.png");
		m_FileIcon = Texture::Create("resources/icons/content-browser/file.png");
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
			if (ImGui::Button("<--")) {
				BackButton();
			}
			ImGui::SameLine();

			if (ImGui::Button("-->")) {
				ForwardButton();
			}
			ImGui::SameLine();

			if (ImGui::Button("Search")) {
				PX_CORE_WARN("The search button doesn't do anything at the moment!\n");
			}

			
			ImGui::SameLine();


			char buffer[255];
			memset(buffer, 0, 255);
			memcpy(buffer, m_CurrentSearchItem.c_str(), m_CurrentSearchItem.length());
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PushItemWidth(350);
			if (ImGui::InputText("##Search", buffer, 255)) {
				m_CurrentSearchItem = buffer;
			}
			ImGui::PopItemWidth();
			ImGui::PopStyleColor(3);


			ImGui::SameLine();

			int dirIterations = 0;

			std::filesystem::path shorcutPath;
			for (auto& p : m_CurrentDirectory) {
				shorcutPath /= p;

				if (dirIterations > 0) {
					ImGui::Text("\\");
					ImGui::SameLine();
				}

				ImGui::PushStyleColor(ImGuiCol_Button, { 1, 1, 1, 0 });
				if (ImGui::Button(p.filename().string().c_str())) {
					m_CurrentDirectory = shorcutPath;
					ImGui::PopStyleColor();
					break;
				}
				ImGui::PopStyleColor();

				ImGui::SameLine();
				dirIterations++;
			}
			ImGui::EndChild();

			if (m_CurrentSearchItem != "") {
				m_SearchForItemMode = true;
			} else {
				m_SearchForItemMode = false;
			}

			

		}

	}

	void EditorContentBrowser::RenderItem(std::filesystem::path path) {


		auto relPath = std::filesystem::relative(path, s_AssetDirectory);
		std::string fileName = relPath.filename().string();

		bool isDirectory = FileSystem::IsDirectory(path);

		Ref<Texture> icon = (isDirectory ? m_FolderIcon : m_FileIcon);
		ImGui::PushID(path.filename().string().c_str());

		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0, 0, 0, 0 });

		ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { m_ThumbnailSize, m_ThumbnailSize }, { 0, 1 }, { 1, 0 });

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
			std::string dragID;
			if (isDirectory) {
				dragID = "DirectoryPayload";
			} else {
				// TODO: Determine what type of asset this is.

				dragID = "AssetPayload";
			}

			// TODO: replace this with the asset manager by using asset handles instead of paths
			std::string relPath = path.string();
			int length = strlen(relPath.c_str());


			ImGui::SetDragDropPayload(dragID.c_str(), relPath.c_str(), strlen(relPath.c_str()));
			ImGui::Text("Path: %s", path.relative_path().string().c_str());
			ImGui::EndDragDropSource();
		}


		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* dirPayload = ImGui::AcceptDragDropPayload("DirectoryPayload"))
			{

				std::string directory;
				
				directory = static_cast<const char*>(dirPayload->Data);
				directory.resize(dirPayload->DataSize);

				
				PX_CORE_MSG("Moved %s into %s\n", directory.c_str(), path.string().c_str());
				FileSystem::MoveDirectory(directory, path.string());
			}
			else if (const ImGuiPayload* dirPayload = ImGui::AcceptDragDropPayload("AssetPayload")) {
				std::string directory;

				directory = static_cast<const char*>(dirPayload->Data);
				directory.resize(dirPayload->DataSize);


				PX_CORE_MSG("Moved %s into %s\n", directory.c_str(), path.string().c_str());
				FileSystem::MoveFile(directory, path.string());
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered()) {
			if (isDirectory) {
				m_CurrentDirectory /= path.filename();

				if (!m_PreviousDirectories.empty()) {
					m_PreviousDirectoryIndex = -1;
				}

			}
			else {
				FileSystem::OpenFileExternally(path);

			}


			m_SearchForItemMode = false;
			m_CurrentSearchItem = "";

		}

		ImGui::PopStyleColor();
		float thumbnailXPosition = ImGui::GetCursorPosX();

		if (ImGui::IsItemHovered() && m_IfHoveredOverItem) {
			if (Pixelate::Input::IsMouseButtonDown(MouseButton::Right)) {
				m_CurrentContextItem = s_AssetDirectory / relPath;
				m_CurrentFileName = fileName;

				m_OpenContextPopup = true;
			}
		}

		ImVec2 texSize = ImGui::CalcTextSize(fileName.c_str());
		float fileNameTextPosition = (thumbnailXPosition + m_ThumbnailSize / 2) - (texSize.x / 2);

		bool textShouldWrap = (fileNameTextPosition < thumbnailXPosition);


		if (m_RenameMode) {
			if (m_CurrentFileName == fileName) {
				char buffer[255];
				memset(buffer, 0, 255);
				memcpy(buffer, m_CurrentFileName.c_str(), m_CurrentFileName.length());
				std::string temp = "##" + m_CurrentFileName;
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

				ImGui::InputText(temp.c_str(), buffer, 255);

				ImGui::PopStyleColor(3);

				if (Input::IsKeyDown(KeyCode::Enter) || (ImGui::IsMouseDoubleClicked(0) && !ImGui::IsItemHovered())) {
					std::filesystem::path renamedPath = path.relative_path().replace_filename(buffer);
					RenameItem(renamedPath.string());
					m_RenameMode = false;
				}
			}
			else {
				if (!textShouldWrap) {
					ImGui::SetCursorPosX(fileNameTextPosition);
					ImGui::Text(fileName.c_str());

				}
				else
					ImGui::TextWrapped(fileName.c_str());

			}
		}
		else {
			if (!textShouldWrap) {
				ImGui::SetCursorPosX(fileNameTextPosition);
				ImGui::Text(fileName.c_str());
			}
			else
				ImGui::TextWrapped(fileName.c_str());

		}

		ImGui::PopID();

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

	void EditorContentBrowser::BackButton()
	{
		if (m_CurrentDirectory != s_AssetDirectory) {
			m_PreviousDirectoryIndex++;
			m_PreviousDirectories.emplace(m_PreviousDirectories.begin() + m_PreviousDirectoryIndex, m_CurrentDirectory);

			m_CurrentDirectory = m_CurrentDirectory.parent_path();
		}
	}

	void EditorContentBrowser::ForwardButton()
	{
		if (!m_PreviousDirectories.empty()) {
			m_CurrentDirectory = m_PreviousDirectories[m_PreviousDirectoryIndex];
			m_PreviousDirectories.erase(m_PreviousDirectories.begin() + m_PreviousDirectoryIndex);
			m_PreviousDirectoryIndex--;
			if (m_PreviousDirectoryIndex <= -1)
				m_PreviousDirectoryIndex = -1;
		}
	}

	void EditorContentBrowser::OnImguiRender()
	{
		bool show = true;
		ImGui::ShowDemoWindow(&show);

		if (!ImGui::Begin("Content Browser")) {
			ImGui::End();
		}
		else {
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

				float cellSize = m_ThumbnailSize + m_Padding;
				int Columns = (int)(ImGui::GetContentRegionAvail().x / cellSize);
				if (Columns < 1)
					Columns = 1;


				ImGui::Columns(Columns, 0, false);


				for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {

					if (!m_SearchForItemMode) {
						RenderItem(entry.path());
						ImGui::NextColumn();

					}
					else {
						std::filesystem::path itemDir = m_CurrentDirectory / m_CurrentSearchItem;
						if (strstr(entry.path().string().c_str(), itemDir.string().c_str())) {
							RenderItem(entry.path());
							ImGui::NextColumn();
						}
					}


				}

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


				ImGui::Columns(1);
				ImGui::EndChild();
				ImGui::SliderFloat("Thumbnail Size", &m_ThumbnailSize, 64.0f, 512.0f, "%.2f");


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

				ImGui::End();


			}

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