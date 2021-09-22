#pragma once

#include "EditorPanel.h"


namespace Pixelate {


	class EditorPhysicsPropertiesPanel : public EditorPanel {


		public:
			void OnImguiRender() override;
		private :
			std::unordered_map<std::string, int16_t> m_FilterIDs;
			std::string m_NewFilterName;

	};


}