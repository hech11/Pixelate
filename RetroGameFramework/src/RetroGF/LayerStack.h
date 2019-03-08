#pragma once


// The LayerStack will contain all the layers under some data structure.
// The LayerStack will iterate "OnUpdate" and "OnEvent" in the "Application.h".


#include "RetroGF/Core.h"
#include "RetroGF/Layer.h"
#include <vector>

namespace RGF {

	class RGF_API LayerStack {
		public :
			LayerStack();
			~LayerStack();

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* overlay);

			void PopLayer();
			void PopOverlay();
			
			void PopLayer(Layer* layer);
			void PopOverlay(Layer* layer);


			inline const std::vector<Layer*>& GetLayerStack() const { return m_LayerStack; }

		private :
			std::vector<Layer*> m_LayerStack;
			unsigned int m_OverLayIndex = 0; // This is needed to let the layerstack know where to push the overlay's in the vector.
	};

}