#include "RGFpch.h"
#include "RigidBody.h"

#include "Physics.h"



namespace RGF {



	RigidBody::RigidBody(const RigidBodyDef& def)
		: m_Def(def)
	{
		b2World* world = (b2World*)Physics::World();

		b2BodyDef d;
		d.position.Set(def.StartPosition.x, def.StartPosition.y);
		d.angle = m_Def.Angle;
		d.type = (b2BodyType)def.Type;
		d.bullet = (def.DetectionMode == CollisionDetectionMode::Continuous ? true : false);
		d.allowSleep = (def.State == SleepingState::NeverSleep ? true : false);
		d.fixedRotation = !def.CanRotate;
		m_BodyData = world->CreateBody(&d);

		b2PolygonShape boxShape;
		boxShape.SetAsBox(def.BoxStartSize.x * 0.5f, def.BoxStartSize.y * 0.5f);

		b2FixtureDef fDef;
		fDef.shape = &boxShape;
		fDef.density = 1;

		const auto& bod = (b2Body*)m_BodyData;
		m_FixtureData = bod->CreateFixture(&fDef);

	}

	RigidBody::~RigidBody()
	{
		b2World* world = (b2World*)Physics::World();
		const auto& bod = (b2Body*)m_BodyData;
		world->DestroyBody(bod);
	}

	void RigidBody::SetLinearVelocity(const glm::vec2& velocity)
	{
		const auto& bod = (b2Body*)m_BodyData;
		bod->SetLinearVelocity({ velocity.x, velocity.y });
	}

	glm::vec3 RigidBody::GetPosition() const {
		const auto& bod = (b2Body*)m_BodyData;
		return { bod->GetPosition().x, bod->GetPosition().y, 0.0f };
	}

}