#pragma once

#include <entt/entt.hpp>

#include "Components.h"
#include "Scene.h"

#include <functional>



namespace RGF {

	class Entity {
		public :
			Entity() = default;
			Entity(entt::entity entHandle, Scene* scene) 
				: m_EntityHandle(entHandle), m_SceneRef(scene)  {}

			template<typename T, typename... Args>
			T& AddComponent(Args&&... args) {
				return m_SceneRef->GetReg().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			}
			template<typename T>
			void RemoveComponent() {
				m_SceneRef->GetReg().remove<T>(m_EntityHandle);
			}


			template<typename T, typename... Args>
			T& GetComponent() {
				return m_SceneRef->GetReg().get<T>(m_EntityHandle);
			}

			template<typename T>
			bool HasComponent() {
				return m_SceneRef->GetReg().has<T>(m_EntityHandle);
			}

			operator unsigned int() const { return (unsigned int)m_EntityHandle; }
			entt::entity GetHandle() { return m_EntityHandle; }
		private :
			entt::entity m_EntityHandle;
			// more research: Should this be a ref if the ownership of this object is always going to be on the scene and therefore destroyed?
			Scene* m_SceneRef; 
	};

}