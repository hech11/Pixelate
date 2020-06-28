#include "RGFpch.h"
#include "Physics.h"

#include "box2d/box2d.h"


#include "RetroGF/Rendering/Renderer2D.h"
#include "RetroGF/Core/Application.h"

namespace RGF {

	static Physics::PhysicsWorldProperties s_Properties;

	struct PhysicsWorldData {
		Scoped<b2World> World;

	};

	PhysicsWorldData s_Data;

	void Physics::Init(const PhysicsWorldProperties& props) {
		s_Properties = props;
		b2Vec2 grav = { s_Properties.Gravity.x, s_Properties.Gravity.y };

		s_Data.World = CreateScoped<b2World>(grav);

	}

	void Physics::Update() {
		s_Data.World->Step(s_Properties.FixedTimeStep, s_Properties.VelocityIterations, s_Properties.PositionIterations);

		// TODO: This is temp. When I decide to start developing the editor, this code will be removed
		Renderer2D::BeginScene(&Application::GetApp().GetPhysicsDebugDraw().GetCamera()->GetCamera());
		s_Data.World->DebugDraw();
		Renderer2D::EndScene();
	}

	void* Physics::World() {
		return s_Data.World.get();
	}

	void Physics::SetProperties(const PhysicsWorldProperties& props)
	{
		s_Properties = props;
	}

	Physics::PhysicsWorldProperties Physics::GetProperties() {
		return s_Properties;
	}

}