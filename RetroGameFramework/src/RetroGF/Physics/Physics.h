#pragma once

#include "GLM/glm/glm.hpp"
#include "RigidBody.h"

#include "PhysicsDebugDraw.h"


namespace RGF {



	class Physics {
		public :

			struct PhysicsWorldProperties {
				float FixedTimeStep = 1.0f / 60.0f;
				int VelocityIterations = 8;
				int PositionIterations = 4;

				glm::vec2 Gravity = glm::vec2( 0.0f, -9.8f);

			};

			static void Init(const PhysicsWorldProperties& props = PhysicsWorldProperties());
			static void Update(float time); //TODO: Application::GetTimer();


			static void* World();

			static void DrawDebugObjects();

			static void SetProperties(const PhysicsWorldProperties& props);
			static PhysicsWorldProperties GetProperties();
			static PhysicsDebugDraw& GetDebug();
	};

}