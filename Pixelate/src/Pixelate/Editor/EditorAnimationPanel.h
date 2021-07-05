#pragma once


#include "Pixelate/Events/Event.h"
#include "Pixelate/Scene/Entity.h"

namespace Pixelate {

	class EditorAnimationPanel {

		public :
			EditorAnimationPanel(){}

			void OnUpdate(float ts){}
			void OnEvent(Event& e){}

			void SetEntityContext(const Entity& entity){}
			void SetEntityContext(){}

			void OnImguiRender(){}

		private :
			Entity m_CurrentEntity = {};
			bool m_SelectedAnEntity = false;
	};

}