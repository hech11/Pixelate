#pragma once

#include <GLM/glm/glm.hpp>
#include "GLM/glm/gtx/transform.hpp"


namespace RGF {



	struct OrthographicCameraBounds {
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCamera {

		public:

			OrthographicCamera() = default;
			OrthographicCamera(const OrthographicCameraBounds& bounds, float orthoSize = 1.0f);

			inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
			inline const glm::mat4& GetViewProjectionMatrix() const { return m_ProjectionViewMatrix; }
			inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }


			void SetProjection(float left, float right, float bottom, float top);
			void SetProjection(const OrthographicCameraBounds& bounds);

			void SetViewMatrix(const glm::mat4& matrix);

			void SetOrthographicSize(float size);
			const float& GetOrthographicSize() const { return m_OrthographicSize; }

			void SetPosition(const glm::vec3& pos);
			void Move(const glm::vec3& offset);

			void SetRotation(float angle); // rotate around the z axis


			void Resize(float aspectRatio);

			glm::vec3& GetPos() { return m_Pos; }
			float& GetAngle() { return m_Angle; }

		private:
			void RecalculateViewProjMatrix();

		protected :
			float m_OrthographicSize = 1.0f;
			float m_AspectRatio;
			float m_Angle;
			glm::vec3 m_Pos;

			OrthographicCameraBounds m_Bounds;
			glm::mat4 m_ViewMatrix;
			glm::mat4 m_ProjectionViewMatrix;
			glm::mat4 m_ProjectionMatrix;

		};


}