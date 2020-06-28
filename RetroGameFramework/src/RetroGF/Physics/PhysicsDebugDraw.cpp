#include "RGFpch.h"
#include "PhysicsDebugDraw.h"


#include "RetroGF/Rendering/Renderer2D.h"

#include "Physics.h"

namespace RGF {


	void PhysicsDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {

	}

	void PhysicsDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
		std::array<glm::vec4, 4> vertex = {
			glm::vec4(vertices[0].x, vertices[0].y, 0.0f, 0.0f),
			glm::vec4(vertices[1].x, vertices[1].y, 0.0f, 0.0f),
			glm::vec4(vertices[2].x, vertices[2].y, 0.0f, 0.0f),
			glm::vec4(vertices[3].x, vertices[3].y, 0.0f, 0.0f)
		};
		glm::vec4 col = { color.r, color.g, color.b, 0.25f };

		Renderer2D::DrawVerticies(vertex, vertexCount, col);


	}

	void PhysicsDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
	{
	}

	void PhysicsDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
	{
	}

	void PhysicsDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
	}

	void PhysicsDebugDraw::DrawTransform(const b2Transform& xf) {
	}

	void PhysicsDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
	{
	}



	void PhysicsDebugDraw::RenderObjects() {
		Renderer2D::BeginScene(&m_Camera->GetCamera());
		const auto& world = (b2World*)Physics::World();
		world->DebugDraw();
		Renderer2D::EndScene();
	}

}