#pragma once


#include "EditorPanel.h"
#include "Pixelate/Rendering/API/Texture.h"

#include <filesystem>
#include "Pixelate/Utility/FileSystem.h"

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
			void CreateItem(AssetType type);

			void RenderDirectory(const std::filesystem::path& dir);
			void RenderTopBar();

			void RenderItem(std::filesystem::path path);

			void DrawItemContext();
			void DrawWindowContext();

			void ForwardButton();
			void BackButton();


			void OnFileWatcherAction(FileWatcherCallbackData data);
		private :

			bool m_DeleteItem = false;
			bool m_RenameMode = false;
			bool m_SearchForItemMode = false;
			bool m_IfHoveredOverItem = false;
			bool m_OpenContextPopup = false;
			bool m_OpenWindowContextPopup = false;

			std::filesystem::path m_CurrentDirectory;
			std::filesystem::path m_CurrentContextItem;
			std::filesystem::path m_CurrentCopiedItem = "";

			std::vector<std::filesystem::path> m_PreviousDirectories;
			int32_t m_PreviousDirectoryIndex = -1;

			std::string m_CurrentFileName;
			std::string m_CurrentSearchItem = "";

			Ref<Texture> m_FolderIcon;
			Ref<Texture> m_FileIcon;


			float m_ThumbnailSize = 64.0f;
			float m_Padding = 16.0f;

	};
}