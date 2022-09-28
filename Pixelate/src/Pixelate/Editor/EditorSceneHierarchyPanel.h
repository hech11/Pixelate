#pragma once

#include "Pixelate/Scene/Scene.h"
#include "Pixelate/Scene/Entity.h"

#include "Pixelate/Editor/EditorCamera.h"
#include "Pixelate/Editor/EditorPanel.h"

namespace Pixelate {

	class EditorSceneHierarchyPanel : public EditorPanel {
		public :

			EditorSceneHierarchyPanel(const Ref<Scene>& scene);

			void OnImguiRender() override;

		private :
			// TODO: abstact this into its own class?

			void RenderMaterialProperty(Ref<Material>& material, ShaderMember prop, int binding);

	};

}