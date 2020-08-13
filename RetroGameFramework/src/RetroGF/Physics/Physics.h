#pragma once

#include "GLM/glm/glm.hpp"
#include "RigidBody.h"

#include "box2d/box2d.h"
#include "PhysicsDebugDraw.h"


namespace RGF {


	struct PhysicsWorldComponent {

		PhysicsWorldComponent() {
			World = CreateScoped<b2World>(b2Vec2(Gravity.x, Gravity.y));
			World->SetDebugDraw(&DebugDraw);
		}


		float FixedTimeStep =  1.0f / 60.0f;
		int VelocityIterations = 8;
		int PositionIterations = 4;
		glm::vec2 Gravity = glm::vec2(0.0f, -9.8f);
		float CurrentSimulationTime = 0.0f;

		Scoped<b2World> World; 
		PhysicsDebugDraw DebugDraw;

		std::vector<RigidBody> AllRigidbodiesInScene;

	};
	
}