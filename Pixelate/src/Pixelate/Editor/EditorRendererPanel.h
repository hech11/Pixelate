#pragma once

#include "EditorPanel.h"
#include "Pixelate/Events/Event.h"
#include "Pixelate/Scene/Entity.h"

namespace Pixelate {

	class EditorRendererPanel : public EditorPanel {

		public:

			void OnImguiRender() override;

			void RenderStats();
			void RenderSettings();
			void RenderShaderProperties();

			void RenderLayers();

		private :
			std::string m_NewLayerName;
	};



}