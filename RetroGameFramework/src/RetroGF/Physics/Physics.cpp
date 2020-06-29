#include "RGFpch.h"
#include "Physics.h"

#include "box2d/box2d.h"


#include "RetroGF/Rendering/Renderer2D.h"
#include "RetroGF/Core/Application.h"

namespace RGF {

	static Physics::PhysicsWorldProperties s_Properties;

	struct PhysicsWorldData {
		b2World* World;
		PhysicsDebugDraw DebugDraw;

		float CurrentSimulationTime = 0.0f;
	};

	PhysicsWorldData s_Data;

	void Physics::Init(const PhysicsWorldProperties& props) {
		s_Properties = props;
		b2Vec2 grav = { s_Properties.Gravity.x, s_Properties.Gravity.y };

		s_Data.World = new b2World(grav);

		s_Data.World->SetDebugDraw(&s_Data.DebugDraw);
		s_Data.DebugDraw.SetFlags(b2Draw::e_shapeBit);

	}



	void Physics::Update(float time) {

		while (s_Data.CurrentSimulationTime < time) {
			s_Data.World->Step(s_Properties.FixedTimeStep, s_Properties.VelocityIterations, s_Properties.PositionIterations);
			s_Data.CurrentSimulationTime += s_Properties.FixedTimeStep;
		}

	}

	void Physics::DrawDebugObjects() {
		// TODO: This is temp. When I decide to start developing the editor, this code will be removed
		s_Data.DebugDraw.RenderObjects();
	}



	void* Physics::World() {
		return s_Data.World;
	}

	void Physics::SetProperties(const PhysicsWorldProperties& props)
	{
		s_Properties = props;
	}

	Physics::PhysicsWorldProperties Physics::GetProperties() {
		return s_Properties;
	}


	PhysicsDebugDraw& Physics::GetDebug() {
		return s_Data.DebugDraw;
	}

}