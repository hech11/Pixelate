#include "RGFpch.h"
#include "PhysicsShapeColliders.h"

#include "BOX2D/box2d.h"
#include "Physics.h"


namespace RGF {



	BoxCollider::BoxCollider(const BoxColliderDef& def)
		: m_Def(def)
	{
		m_Type = ColliderType::Box;
		b2PolygonShape* shape = new b2PolygonShape;
		m_ColliderData = shape;
		shape->SetAsBox(def.Size.x * 0.5f, def.Size.y * 0.5f, { def.Centre.x, def.Centre.y }, def.Angle);
	}


	b2Shape* BoxCollider::GetShapeData() {
		return (b2PolygonShape*)m_ColliderData;
	}


	BoxCollider::~BoxCollider()
	{
		delete m_ColliderData;
	}



}