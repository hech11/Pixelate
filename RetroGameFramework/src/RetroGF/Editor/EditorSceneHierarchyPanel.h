#pragma once

#include "RetroGF/Scene/Scene.h"
#include "RetroGF/Scene/Entity.h"

#include "RetroGF/Editor/EditorCamera.h"

namespace RGF {

	class EditorSceneHierarchyPanel {
		public :

			EditorSceneHierarchyPanel(const Ref<Scene>& scene);
			void SetSceneContext(const Ref<Scene>& scene);
			void SetSelectedEntity(Entity entity);
			void SetSelectedEntity();

			void OnUpdate(float ts, const Ref<EditorCamera>& camera);
			void OnEvent();

			void OnImguiRender();

			Entity& CurrentlySelectedEntity() { return m_CurrentlySelectedEntity; };
			bool& HasAnEntitySelected() { return m_SelectedEntity; };

		private :
			Ref<Scene> m_SceneContext;
			Entity m_CurrentlySelectedEntity;
			bool m_SelectedEntity = false;

	};

}