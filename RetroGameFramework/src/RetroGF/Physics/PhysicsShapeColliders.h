#pragma once

#include "GLM/glm/glm.hpp"

#include "BOX2D/box2d.h"


// The colliders and RigidBody is temp. I will eventually implement a ecs system that will change
// how this API works and looks.


namespace RGF {

	// For now physics shapes only supports boxes and circles
	enum class ColliderType {
		Box,
	};

	struct BoxColliderDef {
		glm::vec2 Size = { 1.0f, 1.0f };
		glm::vec2 Centre = { 0.0f, 0.0f };
		float Angle = 0.0f;
		
	};

	class PhysicsShapeColliders {

		public :
			virtual ~PhysicsShapeColliders() = default;
			virtual b2Shape* GetShapeData() = 0;


			ColliderType& GetColliderType() { return m_Type; }
		public :
			bool IsTrigger = false;
			float Density = 1.0f;
			float Friction = 0.2f;

		protected:
			b2Shape* m_ColliderData;
			ColliderType m_Type;
	};


	class BoxCollider : public PhysicsShapeColliders {

		public :

			BoxCollider(const BoxColliderDef& def);
			~BoxCollider();

			b2Shape* GetShapeData() override;

		private :
			BoxColliderDef m_Def;
	};

}