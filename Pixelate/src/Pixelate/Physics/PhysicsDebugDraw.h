#pragma once


#include <box2d/box2d.h>

#include "Pixelate/Rendering/OrthographicCamera.h"
#include "Pixelate/Core/Core.h"

// Not being used... 

namespace Pixelate {
	class PhysicsDebugDraw : public b2Draw {

		public:

			// Mimics box2d's flags
			enum DrawFlag {
				shapeBit = BIT(0),	///< draw shapes
				jointBit = BIT(1),	///< draw joint connections
				aabbBit = BIT(2),	///< draw axis aligned bounding boxes
				pairBit = BIT(3),	///< draw broad-phase pairs
				centerOfMassBit = BIT(4) ///< draw center of mass frame
			};


			void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
			void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
			void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;
			void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;
			void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
			void DrawTransform(const b2Transform& xf) override;
			void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;


			void ShouldDrawVisuals(bool show);
			void SetDrawFlag(unsigned int flag);
		private :
			bool m_ShowVisuals = true;
	};

	inline PhysicsDebugDraw::DrawFlag operator| (PhysicsDebugDraw::DrawFlag a, PhysicsDebugDraw::DrawFlag b) 
	{ return (PhysicsDebugDraw::DrawFlag)((int)a | (int)b); }


}