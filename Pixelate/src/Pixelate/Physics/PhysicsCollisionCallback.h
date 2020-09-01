#pragma once

#include <box2d/box2d.h>

namespace Pixelate {

	class PhysicsCollisionCallback : public b2ContactListener {

		public:
			void BeginContact(b2Contact* contact) override;
			void EndContact(b2Contact* contact) override;

	};

}