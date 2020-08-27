#pragma once

#include "GLM/glm/glm.hpp"
#include "box2d/box2d.h"


namespace Pixelate {

	class Scene;


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

		glm::vec2 Position = glm::vec2(0.0f, 0.0f);
		float Angle = 0.0f;
		float GravityScale = 1.0f;
		SleepingState State = SleepingState::Awake;
		CollisionDetectionMode DetectionMode = CollisionDetectionMode::Discrete;
		BodyType Type = BodyType::Dynamic;

		bool CanRotate = false;
	};



	class RigidBody {
		public :
		

			
			RigidBody() = default;
			RigidBody(Scene* scene, const RigidBodyDef& def = RigidBodyDef());

			~RigidBody();

			void Init(Scene* scene, const RigidBodyDef& def = RigidBodyDef());
			void Destroy();

			void AddCollider(b2Shape* collider, float density, float friction, bool isTrigger);
			void RemoveCollider(b2Shape* collider);

			void SetCollisionDetectionMode(CollisionDetectionMode mode);
			CollisionDetectionMode GetCollisionDetectionMode() const;

			void SetSleepState(SleepingState mode);
			SleepingState GetSleepingState() const;

			void SetBodyType(BodyType type);
			BodyType GetBodyType() const;

			void SetGravityScale(float scale);
			float GetGravityScale() const;

			void ShouldRotate(bool rotate);
			bool CanRotate() const;


			void SetTransform(const glm::vec3& positon, float angle);

			void SetLinearVelocity(const glm::vec2& velocity);
			glm::vec2 GetLinearVelocity() const;


			float GetAngle() const;
			glm::vec3 GetPosition() const;


		private :
			std::vector<b2Fixture*> m_Fixtures;
			RigidBodyDef m_Definition;
			b2Body* m_BodyData;
			b2World* m_World;

	};

}