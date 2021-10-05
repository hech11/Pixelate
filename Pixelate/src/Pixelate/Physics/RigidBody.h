#pragma once

#include "GLM/glm/glm.hpp"
#include "box2d/box2d.h"

#include "PhysicsMaterial2D.h"


namespace Pixelate {


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

		bool FixedRotation = false;

		int16_t CategoryLayer = 0x0001;

		Ref<PhysicsMaterial2D> Material = CreateRef<PhysicsMaterial2D>();


	};

	struct PhysicsWorldComponent;
	class Scene;

	struct CollisionCallBack {
		unsigned int Handle;
		Scene* SceneRef;
	};

	class RigidBody {
		public :
			RigidBody() = default;
			RigidBody(PhysicsWorldComponent& physicsWorld, CollisionCallBack collisionHandle, const RigidBodyDef& def = RigidBodyDef());

			~RigidBody();

			void Init(PhysicsWorldComponent& physicsWorld, CollisionCallBack collisionHandle, const RigidBodyDef& def = RigidBodyDef());
			void Destroy();


			void SetUserData(void* data);
			void* GetUserData();

			void AddCollider(b2Shape* collider, int16_t catergoryLayer, int16_t layerMaskRule, float density, float friction, float bounciness, bool isTrigger);

			void AddCollider(b2Shape* collider, int16_t catergoryLayer, int16_t layerMaskRule, const Ref<PhysicsMaterial2D>& material, bool isTrigger);

			void RemoveCollider(b2Shape* collider);

			void SetCollisionDetectionMode(CollisionDetectionMode mode);
			CollisionDetectionMode GetCollisionDetectionMode() const;

			void SetSleepState(SleepingState mode);
			SleepingState GetSleepingState() const;

			void SetBodyType(BodyType type);
			BodyType GetBodyType() const;

			void SetGravityScale(float scale);
			float GetGravityScale() const;

			void SetFixedRotation(bool rotate);
			bool CanZRotate() const;

			void SetTransform(const glm::vec3& positon, float angle);

			void SetLinearVelocity(const glm::vec2& velocity);
			glm::vec2 GetLinearVelocity() const;


			float GetAngle() const;
			glm::vec3 GetPosition() const;


		private :
			std::vector<b2Fixture*> m_Fixtures;
			b2Body* m_BodyData;
			b2World* m_World;

			CollisionCallBack m_CollisionHandle;
	};

}