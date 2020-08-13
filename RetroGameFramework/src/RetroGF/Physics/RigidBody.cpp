#include "RGFpch.h"
#include "RigidBody.h"

#include "Physics.h"

#include "RetroGF/Debug/Instrumentor.h"
#include "RetroGF/Scene/Scene.h"



namespace RGF {




	RigidBody::RigidBody(Scene* scene, const RigidBodyDef& def) {
		Init(scene, def);
	}

	void RigidBody::Init(Scene* scene, const RigidBodyDef& def) {
		RGF_PROFILE_FUNCTION();
		auto entity = scene->GetSceneEntity();
		auto& physicsWorld = scene->GetReg().get<PhysicsWorldComponent>(entity);
		m_World = physicsWorld.World.get();

		b2BodyDef d;
		d.position.Set(def.Position.x, def.Position.y);
		d.angle = def.Angle;
		d.type = (b2BodyType)def.Type;
		d.bullet = (def.DetectionMode == CollisionDetectionMode::Continuous ? true : false);
		d.allowSleep = (def.State == SleepingState::NeverSleep ? false : true);
		if (d.allowSleep) {
			d.awake = def.State == SleepingState::Awake ? true : false;
		}
		d.fixedRotation = !def.CanRotate;
		d.gravityScale = def.GravityScale;
		m_BodyData = physicsWorld.World->CreateBody(&d);
	}

	void RigidBody::Destroy() {
		RGF_PROFILE_FUNCTION();

		for (auto& Fixture : m_Fixtures) {
			m_BodyData->DestroyFixture(Fixture);
		}
		if(m_BodyData < 0)
			m_World->DestroyBody(m_BodyData);
		m_BodyData = nullptr;
		m_Fixtures.clear();
	}

	RigidBody::~RigidBody()
	{
		Destroy();
	}

	void RigidBody::AddCollider(b2Shape* collider, float density, float friction, bool isTrigger) {
		RGF_PROFILE_FUNCTION();
		b2FixtureDef fDef;

		fDef.shape = collider;
		fDef.density = density;
		fDef.friction = friction;
		fDef.isSensor = isTrigger;

		m_Fixtures.push_back(m_BodyData->CreateFixture(&fDef));
	}

	void RigidBody::RemoveCollider(b2Shape* collider) {
		RGF_PROFILE_FUNCTION();
		int iterator = 0;
		for (auto& Fixture : m_Fixtures) {
			if (Fixture->GetShape() == collider) {
				m_BodyData->DestroyFixture(Fixture);
				m_Fixtures.erase(m_Fixtures.begin() + iterator);
				break;
			}

			iterator++;
		}
	}


	void RigidBody::SetCollisionDetectionMode(CollisionDetectionMode mode) {
		bool bullet = (mode == CollisionDetectionMode::Continuous ? true : false);
		m_BodyData->SetBullet(bullet);
	}

	void RigidBody::SetSleepState(SleepingState mode) {
		if (mode == SleepingState::NeverSleep) {
			m_BodyData->SetSleepingAllowed(false);
			m_BodyData->SetAwake(true);
			return;
		}

		m_BodyData->SetSleepingAllowed(true);
		bool awake = false;
		if (mode == SleepingState::Sleep) {
			awake = false;
		}
		m_BodyData->SetAwake(awake);

	}

	void RigidBody::SetBodyType(BodyType type) {
		b2BodyType b2type{};
		switch (type)
		{
			case RGF::BodyType::Static:
				b2type = b2_staticBody;
				break;
			case RGF::BodyType::Kinematic:
				b2type = b2_kinematicBody;
				break;
			case RGF::BodyType::Dynamic:
				b2type = b2_dynamicBody;
				break;
		}
		m_BodyData->SetType(b2type);
	}

	void RigidBody::SetGravityScale(float scale)
	{
		m_BodyData->SetGravityScale(scale);
	}

	void RigidBody::CanRotate(bool rotate)
	{
		m_BodyData->SetFixedRotation(rotate);
	}

	void RigidBody::SetTransform(const glm::vec3& positon, float angle) {
		m_BodyData->SetTransform({ positon.x, positon.y }, angle);
	}

	void RigidBody::SetLinearVelocity(const glm::vec2& velocity)
	{
		RGF_PROFILE_FUNCTION();
		m_BodyData->SetLinearVelocity({ velocity.x, velocity.y });
	}
	glm::vec2 RigidBody::GetLinearVelocity() const {
		RGF_PROFILE_FUNCTION();
		return { m_BodyData->GetLinearVelocity().x, m_BodyData->GetLinearVelocity().y };
	}

	float RigidBody::GetAngle() const {
		return m_BodyData->GetAngle();
	}

	glm::vec3 RigidBody::GetPosition() const {
		RGF_PROFILE_FUNCTION();
		return { m_BodyData->GetPosition().x, m_BodyData->GetPosition().y, 0.0f };
	}

}