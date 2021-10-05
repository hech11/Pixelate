#pragma once

#include "EditorPanel.h"


namespace Pixelate {


	class EditorPhysicsPropertiesPanel : public EditorPanel {


		public:
			void OnImguiRender() override;

		private :
			void RenderWorldProps();
			void RenderLayerMatrix();

		private :
			std::string m_NewLayerName;

	};


}