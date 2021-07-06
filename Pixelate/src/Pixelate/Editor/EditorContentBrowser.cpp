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

		for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {

			auto relPath = std::filesystem::relative(entry.path(), s_AssetDirectory);

			std::string fileName = relPath.filename().string();
			if (entry.is_directory()) {
				if (ImGui::Button(fileName.c_str())) {
					m_CurrentDirectory /= entry.path().filename();
				}
			}
			else {
				if (ImGui::Button(fileName.c_str())) {
				}
			}


		}


		ImGui::End();
	}

}