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
			void RenameItem();
			void DeleteItem();
		private :

			bool m_DeleteItem = false;
			std::filesystem::path m_CurrentDirectory;
			std::filesystem::path m_CurrentContextItem;

	};
}