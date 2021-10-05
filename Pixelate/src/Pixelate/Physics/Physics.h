#pragma once

#include "GLM/glm/glm.hpp"
#include "RigidBody.h"

#include "box2d/box2d.h"
#include "PhysicsDebugDraw.h"
#include "PhysicsCollisionCallback.h"

#include <bitset>

namespace Pixelate {


	struct PhysicsWorldComponent {

		PhysicsWorldComponent() {
			World = CreateScoped<b2World>(b2Vec2(Gravity.x, Gravity.y));
			World->SetDebugDraw(&DebugDraw);
			World->SetContactListener(&CollisionCallback);
		}


		float FixedTimeStep =  1.0f / 60.0f;
		int VelocityIterations = 8;
		int PositionIterations = 4;
		glm::vec2 Gravity = glm::vec2(0.0f, -9.8f);
		float CurrentSimulationTime = 0.0f;

		Scoped<b2World> World; 
		PhysicsDebugDraw DebugDraw;
		PhysicsCollisionCallback CollisionCallback;

	};


	enum PhysicsLayersIndex : int16_t {
		Layer1  = BIT(0),
		Layer2  = BIT(1),
		Layer3  = BIT(2),
		Layer4  = BIT(3),
		Layer5  = BIT(4),
		Layer6  = BIT(5),
		Layer7  = BIT(6),
		Layer8  = BIT(7),
		Layer9  = BIT(8),
		Layer10 = BIT(9),
		Layer11 = BIT(10),
		Layer12 = BIT(11),
		Layer13 = BIT(12),
		Layer14 = BIT(13),
		Layer15 = BIT(14),
		FinalLayer = Layer15,
	};

	struct PhysicsLayer {

		int16_t ID = 0;
		std::bitset<16> Rule;

		std::string DebugName;

		bool operator==(const PhysicsLayer& other) const {
			return (ID == other.ID && DebugName == other.DebugName && Rule == other.Rule);
		}
	};

	class Physics {

		public :

			static std::vector<PhysicsLayer>& GetLayers() { return s_Layers; }

			static void AddCollisionLayer(const std::string& name);
			static void RemoveCollisionLayer(int16_t layerId);

			static void RenameCollisionLayer(int16_t layerID, const std::string& name);

			static int16_t GetLayerRuleMask(int16_t layerCategory);

			static int GetMaxFilters() { return 15; }
			static std::bitset<16> GenerateDefaultLayerMask() { return 0x7FFF; }


			static std::string& LayerIDToString(int16_t layerID);


		private :
			static std::vector<PhysicsLayer> s_Layers;

	};

	
}
// 
// namespace std {
// 
// 	template <>
// 	struct hash<Pixelate::PhysicsLayer> {
// 
// 		std::size_t operator()(const Pixelate::PhysicsLayer& id) const {
// 
// 			std::string hashData = id.DebugName + std::to_string(id.ID);
// 			size_t hashResult = hash<std::string>()(hashData);
// 
// 			return hashResult;
// 		}
// 	};
// 
// }