#pragma once

#include <entt/entt.hpp>
#include "Pixelate/Events/Event.h"
#include "Pixelate/Editor/EditorCamera.h"
#include "Pixelate/Physics/Physics.h"

#include "glm/glm.hpp"
#include "Pixelate/Core/UUID.h"

namespace Pixelate {

	class Entity;
	class Scene {

		public :
			Scene(const std::string& name = "Scene"); // This should be stripped/not used in final maybe?
			~Scene();

			void Init(); // Does nothing at the moment. This would init a scene from a file saved externally.
			void GenerateDefaultScene(); // This could be moved into some SceneManager class 

			// always update regardless of editor state
			void OnUpdate(float ts, const Ref<EditorCamera>& camera, Entity selectedEntity, bool hasEntityBeenSelected);
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
			std::unordered_map<UUID, Entity>& GetEntityMap() { return m_EntityMap; }

			Entity CreateEntity(const std::string& name = "Entity");
			Entity CreateEntityWithUUID(UUID uuid, const std::string& name = "Entity");
			void DeleteEntity(Entity entity);

			void SetName(const std::string& name) { m_Name = name; }
			const std::string& GetName() const { return m_Name; }

		private :
			entt::entity m_SceneEntity;
			entt::registry m_Reg;
			std::string m_Name;

			std::unordered_map<Pixelate::UUID, Entity> m_EntityMap;

			float m_AspectRatio;

			bool m_IsPlaying = false;

	};

}