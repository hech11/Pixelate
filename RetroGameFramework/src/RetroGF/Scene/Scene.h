#pragma once

#include <entt/entt.hpp>
#include "RetroGF/Events/Event.h"
#include "RetroGF/Editor/EditorCamera.h"

namespace RGF {

	class Entity;
	class Scene {

		public :
			Scene(const std::string& name = "Scene"); // This should be stripped/not used in final maybe?
			~Scene();

			void Init(); // Does nothing at the moment. This would init a scene from a file saved externally.

			void OnUpdate(float ts, EditorCamera& camera);
			void OnEvent(Event& e);
			
			entt::registry& GetReg() { return m_Reg; }


			Ref<Entity> CreateEntity();

		private :
			entt::registry m_Reg;
			std::string m_Name;

	};

}