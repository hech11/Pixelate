#pragma once

#include "RetroGF/Scene/Scene.h"
#include "RetroGF/Scene/Entity.h"

namespace RGF {

	class EditorSceneHierarchyPanel {
		public :

			EditorSceneHierarchyPanel(const Ref<Scene>& scene);
			void SetSceneContext(const Ref<Scene>& scene);
			void SetSelectedEntity(const Ref<Entity>& entity);

			void OnUpdate(float ts);
			void OnEvent();

			void OnImguiRender();

		private :
			Ref<Scene> m_SceneContext;
			Entity m_CurrentlySelectedEntity;
	};

}