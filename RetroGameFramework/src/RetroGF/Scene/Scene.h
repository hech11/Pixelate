#pragma once

#include <entt/entt.hpp>
#include "RetroGF/Events/Event.h"
#include "RetroGF/Editor/EditorCamera.h"
#include "RetroGF/Physics/Physics.h"

#include "glm/glm.hpp"

namespace RGF {

	class Entity;
	class Scene {

		public :
			Scene(const std::string& name = "Scene"); // This should be stripped/not used in final maybe?
			~Scene();

			void Init(); // Does nothing at the moment. This would init a scene from a file saved externally.


			// always update regardless of editor state
			void OnUpdate(float ts, const Ref<EditorCamera>& camera);
			void OnGameViewportRender();

			// Runtime functions
			void OnRuntimeStart();
			void OnRuntimeUpdate(float ts); 
			void OnRuntimeStop();
			
			void SetGameViewport(unsigned int width, unsigned int height);

			entt::registry& GetReg() { return m_Reg; }
			template<typename T>
			auto GetAllEntitiesWith() {
				return m_Reg.view<T>();
			}

			const entt::entity& GetSceneEntity() const { return m_SceneEntity; }

			Entity CreateEntity(const std::string& name = "Entity");
			void DeleteEntity(Entity entity);
		private :

			entt::entity m_SceneEntity;
			entt::registry m_Reg;
			std::string m_Name;

			float m_AspectRatio;

	};

}