#pragma once


#include "EditorPanel.h"

#include <filesystem>

namespace Pixelate {


	

	class EditorContentBrowser : public EditorPanel {


		public:
			EditorContentBrowser();

			void OnImguiRender() override;
		private :
			void ShowCurrentFileLocation();
			void RenameItem(const std::string& newName);
			void DeleteItem();
			void CopyItem();

			void CreateFolder();

			void RenderDirectory(const std::filesystem::path& dir);
			void RenderTopBar();

			void DrawItemContext();
			void DrawWindowContext();

		private :

			bool m_DeleteItem = false;
			bool m_RenameMode = false;
			bool m_IfHoveredOverItem = false;
			bool m_OpenContextPopup = false;
			bool m_OpenWindowContextPopup = false;

			std::filesystem::path m_CurrentDirectory;
			std::filesystem::path m_CurrentContextItem;
			std::filesystem::path m_CurrentCopiedItem = "";
			std::string m_CurrentFileName;

	};
}