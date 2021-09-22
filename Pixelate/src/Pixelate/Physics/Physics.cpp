#include "PXpch.h"
#include "Physics.h"

#include "box2d/box2d.h"


#include "Pixelate/Rendering/Renderer2D.h"
#include "Pixelate/Core/Application.h"

#include "Pixelate/Debug/Instrumentor.h"

namespace Pixelate {

	std::unordered_map<int16_t, std::string> Physics::s_Filters = {
		{PhysicsLayers::Layer1, "Default"},
		{PhysicsLayers::Layer2,  "Empty"},
		{PhysicsLayers::Layer3,  "Empty"},
	};
	std::unordered_map<int16_t, int16_t> Physics::s_FilterRules = { 
		{PhysicsLayers::Layer1,  PhysicsLayers::Layer15},
		{PhysicsLayers::Layer2,  PhysicsLayers::Layer15},
		{PhysicsLayers::Layer3,  PhysicsLayers::Layer15},
	}; 
	int16_t Physics::s_FilterCounter = BIT(0);




	void Physics::AddCollisionFilter(const std::string& name) {
// 		if(s_FilterCounter < 16)
// 			s_FilterCounter += BIT(0);
// 
// 		s_Filters[s_FilterCounter] = name;
	}


	void Physics::RemoveCollisionFilter(const std::string& name) {
// 		if (s_Filters.find(name) == s_Filters.end()) {
// 			PX_CORE_WARN("Cannot find '%s'. This seems to not exist in the collision filter!\n", name.c_str());
// 			return;
// 		}
// 
// 		int16_t rule;
// 		for (auto it = s_Filters.begin(); it != s_Filters.end(); ++it) {
// 
// 			if (it->first == name) {
// 				rule = it->second;
// 				s_Filters.erase(it);
// 				break;
// 			}
// 		}

//		s_FilterRules.erase(rule);
	}

	void Physics::AddFilterRule(const std::string& filterName, int16_t rule)
	{
// 		if (s_Filters.find(filterName) == s_Filters.end()) {
// 			PX_CORE_WARN("Filter name: '%s' does not exist in the collision filter!\n", filterName.c_str());
// 			return;
// 		}
// 
// 		s_FilterRules[s_Filters[filterName]] = rule;

	}

	
	int16_t Physics::UnflipFilter(int16_t mask, int16_t nibbleToSubtract)
	{
		int16_t position = ~nibbleToSubtract;
		int16_t result = position & mask;

		return result;

	}

	int16_t Physics::FlipFilter(int16_t mask, int16_t nibbleToAdd)
	{
		int16_t result = mask | nibbleToAdd;
		return result;

	}

	int16_t Physics::ConvertFilterStringToID(const std::string& name)
	{
		for (auto& [id, n] : s_Filters)
		{
			if (n == name)
				return id;
		}
	}

	std::string Physics::ConvertFilterIDToString(int16_t id)
	{
		if(s_Filters.find(id) != s_Filters.end())
			return s_Filters[id];

		return "Empty";
	}

}