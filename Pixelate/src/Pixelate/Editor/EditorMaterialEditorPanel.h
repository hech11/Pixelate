#pragma once

#include "Pixelate/Events/Event.h"
#include "Pixelate/Scene/Entity.h"

#include "Pixelate/Editor/EditorPanel.h"

namespace Pixelate {

	class EditorMaterialEditorPanel : public EditorPanel {

		public:
			void OnEvent(Event& e) override;
			void OnImguiRender() override;

		private :
			void RenderShaderList();
			void RenderMaterialList();


	};

}