#pragma once

#include "GLM/glm/glm.hpp"
#include "box2d/box2d.h"

#include "PhysicsShapeColliders.h"

namespace RGF {

	class RigidBody {
		public :


			enum class SleepingState {
				Sleep,
				Awake,
				NeverSleep
			};
			enum class CollisionDetectionMode {
				Continuous,
				Discrete
			};
			enum class BodyType {
				Static = 0,
				Kinematic,
				Dynamic
			};

		

			struct RigidBodyDef {

				glm::vec2 StartPosition = glm::vec2(0.0f, 0.0f);
				float Angle = 0.0f;
				float GravityScale = 1.0f;
				RigidBody::SleepingState State = RigidBody::SleepingState::Awake;
				RigidBody::CollisionDetectionMode DetectionMode = RigidBody::CollisionDetectionMode::Discrete;
				RigidBody::BodyType Type = RigidBody::BodyType::Dynamic;

				bool CanRotate = false;
			};

			RigidBody(const RigidBodyDef& def = RigidBodyDef());
			~RigidBody();

			void AddCollider(PhysicsShapeColliders* collider);
			void RemoveCollider(PhysicsShapeColliders* collider);

			void SetLinearVelocity(const glm::vec2& velocity);
			glm::vec2 GetLinearVelocity() const;

			glm::vec3 GetPosition() const;
		private :

			RigidBodyDef m_Def;
			std::vector<b2Fixture*> m_Fixtures;
			b2Body* m_BodyData;

	};

}