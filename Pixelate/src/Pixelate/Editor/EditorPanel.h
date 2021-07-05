#pragma once


// To be inherited by editor panels. Used as a quick way to create them


#include "Pixelate/Scene/Entity.h"
#include "Pixelate/Scene/Scene.h"
#include "Pixelate/Events/Event.h"

namespace Pixelate {


	class EditorPanel {
		public :

			virtual ~EditorPanel() = default;


			virtual void OnUpdate(float ts) {}
			virtual void OnEvent(Event& e) {}
			virtual void OnImguiRender() {}

			virtual void SetOpenPanel(bool open) { m_IsPanelOpen = open; }

			virtual void SetSceneContext(const Ref<Scene>& scene) {
				m_SceneContext = scene;
				m_HasSelectedEntity = false;
			}
			virtual void SetSelectedEntity(const Entity& entity) {
				m_CurrentlySelectedEntity = entity;
				m_HasSelectedEntity = ((entity == Entity()) ? false : true);
			}


			Entity& CurrentlySelectedEntity() { return m_CurrentlySelectedEntity; };
			bool& HasAnEntitySelected() { return m_HasSelectedEntity; };


		protected:
			Ref<Scene> m_SceneContext;
			Entity m_CurrentlySelectedEntity;
			bool m_HasSelectedEntity = false;

			bool m_IsPanelOpen = true;


	};

	class EditorPanelManager {
		public :

			EditorPanelManager(){}

			void RegisterPanel(const std::string& panelName, const Ref<EditorPanel>& panel, bool openByDefault = true)
			{
				panel->SetOpenPanel(openByDefault);
				m_Panels[panelName] = panel; 
			}

			void OnUpdate(float ts) { 
				auto it = m_Panels.begin();
				while (it != m_Panels.end())
				{
					it->second->OnUpdate(ts);
					it++;
				}

			}
			void OnEvent(Event& e) {
				
				auto it = m_Panels.begin();
				while (it != m_Panels.end())
				{
					it->second->OnEvent(e);
					it++;
				}
			
			}
			void OnImguiRender() {
				auto it = m_Panels.begin();
				while (it != m_Panels.end())
				{
					it->second->OnImguiRender();
					it++;
				}
			}

			void SetSceneContext(const Ref<Scene>& scene) {
				auto it = m_Panels.begin();
				while (it != m_Panels.end())
				{
					it->second->SetSceneContext(scene);
					it++;
				}
			}

			void SetSelectedEntity(const Entity& entity) {
				auto it = m_Panels.begin();
				while (it != m_Panels.end())
				{
					it->second->SetSelectedEntity(entity);
					it++;
				}
			}


			Ref<EditorPanel>& GetPanel(const std::string& panel) { return m_Panels[panel]; }

		public :
			static EditorPanelManager& Get() { static EditorPanelManager s_Instance; return s_Instance; }

		private :
			std::unordered_map<std::string, Ref<EditorPanel>> m_Panels;
	};
}
