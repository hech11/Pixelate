#pragma once


#include "EditorPanel.h"

#include <filesystem>

namespace Pixelate {

	class EditorContentBrowser : public EditorPanel {


		public:
			EditorContentBrowser();

			void OnImguiRender() override;
		private :
			std::filesystem::path m_CurrentDirectory;
	};
}