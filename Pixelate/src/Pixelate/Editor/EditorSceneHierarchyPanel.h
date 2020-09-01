#pragma once

#include "Pixelate/Scene/Scene.h"
#include "Pixelate/Scene/Entity.h"

#include "Pixelate/Editor/EditorCamera.h"

namespace Pixelate {

	class EditorSceneHierarchyPanel {
		public :

			EditorSceneHierarchyPanel(const Ref<Scene>& scene);
			void SetSceneContext(const Ref<Scene>& scene);
			void SetSelectedEntity(Entity entity);
			void SetSelectedEntity();


			void OnImguiRender();

			Entity& CurrentlySelectedEntity() { return m_CurrentlySelectedEntity; };
			bool& HasAnEntitySelected() { return m_SelectedEntity; };

		private :
			Ref<Scene> m_SceneContext;
			Entity m_CurrentlySelectedEntity;
			bool m_SelectedEntity = false;

	};

}