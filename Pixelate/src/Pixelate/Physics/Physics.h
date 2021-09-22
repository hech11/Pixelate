#pragma once

#include "GLM/glm/glm.hpp"
#include "RigidBody.h"

#include "box2d/box2d.h"
#include "PhysicsDebugDraw.h"
#include "PhysicsCollisionCallback.h"


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

		std::vector<int16_t> Filters;




	};


	enum PhysicsLayers : int16_t {
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
	};

	class Physics {

		public :

			static std::unordered_map<int16_t, std::string>& GetFilters() { return s_Filters; }
			static std::unordered_map<int16_t, int16_t>& GetFilterRules() { return s_FilterRules; }

			static void AddCollisionFilter(const std::string& name);
			static void RemoveCollisionFilter(const std::string& name);

			static void AddFilterRule(const std::string& filterName, int16_t rule);

			static int16_t UnflipFilter(int16_t mask, int16_t nibbleToSubtract);
			static int16_t FlipFilter(int16_t mask, int16_t nibbleToAdd);


			static int16_t ConvertFilterStringToID(const std::string& name);
			static std::string ConvertFilterIDToString(int16_t id);

		private :
			static std::unordered_map<int16_t, std::string> s_Filters;
			static std::unordered_map<int16_t, int16_t> s_FilterRules;

			static int16_t s_FilterCounter;

	};

	
}