#include "PXpch.h"
#include "LayerStack.h"


#include "Pixelate/Debug/Instrumentor.h"



namespace Pixelate {

	LayerStack::LayerStack() : m_OverLayIndex(0){
	}


	LayerStack::~LayerStack() {
		for (Layer* layer : m_LayerStack) {
			layer->ShutDown();
			delete layer;
		}
	} 


	void LayerStack::PushLayer(Layer* layer) {
		PX_PROFILE_SCOPE("LayerStack::PushLayer::" + layer->GetName() + "::Init");

		layer->Init();
		m_LayerStack.emplace(m_LayerStack.end() - m_OverLayIndex, layer);
	}
	void LayerStack::PushOverlay(Layer* overlay) {
		PX_PROFILE_SCOPE("LayerStack::PushOverlay::" + overlay->GetName() + "::Init");

		overlay->Init();
		m_LayerStack.emplace_back(overlay);
	}

	void LayerStack::PopLayer() {
		m_LayerStack.erase(m_LayerStack.end() - m_OverLayIndex);

	}
	void LayerStack::PopOverlay() {
		m_LayerStack.pop_back();
		m_OverLayIndex--;
	}

	void LayerStack::PopLayer(Layer* layer) {
		const auto& index = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);
		if (index != m_LayerStack.end()) {
			m_LayerStack.erase(index);
		}
	}
	void LayerStack::PopOverlay(Layer* layer) {
		const auto& index = std::find(m_LayerStack.begin(), m_LayerStack.end(), layer);
		if (index != m_LayerStack.end()) {
			m_LayerStack.erase(index);
			m_OverLayIndex--;
		}
	}

}