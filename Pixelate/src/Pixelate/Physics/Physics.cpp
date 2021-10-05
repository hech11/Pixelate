#include "PXpch.h"
#include "Physics.h"

#include "box2d/box2d.h"


#include "Pixelate/Rendering/Renderer2D.h"
#include "Pixelate/Core/Application.h"

#include "Pixelate/Debug/Instrumentor.h"

namespace Pixelate {


	static int16_t s_LayerCounter = BIT(0);

	std::vector<PhysicsLayer> Physics::s_Layers = {
		{PhysicsLayersIndex::Layer1, GenerateDefaultLayerMask() , "Default"},
		{PhysicsLayersIndex::Layer2, GenerateDefaultLayerMask() , "Player"},
	};
	

	void Physics::AddCollisionLayer(const std::string& name)
	{

		if (s_LayerCounter < PhysicsLayersIndex::FinalLayer) {
			s_LayerCounter += BIT(0);

			PhysicsLayer layer = { s_LayerCounter, GenerateDefaultLayerMask(), name };
			s_Layers.emplace_back(layer);

		} else {
			PX_CORE_WARN("We cannot add another phyics layer. We only support up to 15 physics layers!\n");
		}

	}

	void Physics::RemoveCollisionLayer(int16_t layerId)
	{
		for (int i = 0; i < s_Layers.size(); i++) {
			if (s_Layers[i].ID == layerId) {
				s_Layers.erase(s_Layers.begin() + i);
				break;
			}
		}
	}

	void Physics::RenameCollisionLayer(int16_t layerID, const std::string& name)
	{
		for (auto& layer : s_Layers) {

			if (layer.ID == layerID) {
				layer.DebugName = name;
				break;
			}
		}

	}


	int16_t Physics::GetLayerRuleMask(int16_t layerCategory)
	{
		for (auto& layer : s_Layers) {

			if (layer.ID == layerCategory) {
				return (uint16_t)layer.Rule.to_ulong();
			}
		}
	}

	std::string& Physics::LayerIDToString(int16_t layerID)
	{
		for (auto & layer : s_Layers) {

			if (layer.ID == layerID) {
				return layer.DebugName;
			}
		}
	}

}