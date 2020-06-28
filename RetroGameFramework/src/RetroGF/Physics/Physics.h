#pragma once

#include "GLM/glm/glm.hpp"

namespace RGF {



	class Physics {
		public :

			struct PhysicsWorldProperties {
				float FixedTimeStep = 1 / 60.0f;
				int VelocityIterations = 8;
				int PositionIterations = 3;

				glm::vec2 Gravity = glm::vec2( 0.0f, -9.8 );

			};

			static void Init(const PhysicsWorldProperties& props = PhysicsWorldProperties());
			static void Update();

			static void* World();

			static void SetProperties(const PhysicsWorldProperties& props);
			static PhysicsWorldProperties GetProperties();

	};

}