#pragma once

#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtx/transform.hpp"

namespace RGF {


	class Camera {

		public :
			Camera()
				: m_ViewMatrix(glm::mat4(1.0f)), m_ProjectionMatrix(glm::mat4(1.0f)), m_Pos({ 0.0f, 0.0f, 0.0f }), m_Rot({1.0f, 1.0f, 1.0f}),
				m_Scale({1.0f, 1.0f, 1.0f}), m_Angle(0.0f) {}

			inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
			inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

			void Init(const glm::mat4& view, const glm::mat4& projection) {
				m_ViewMatrix = view;
				m_ProjectionMatrix = projection;

				m_ProjectionViewMatrix = view * projection;
			}

			void SetPosition(const glm::vec3& pos) {
				m_Pos = pos;
				UpdateViewMatrix();
			}
			void SetRotation(float angle, const glm::vec3& axis) {
				m_Rot = axis;
				m_Angle = angle;
				UpdateViewMatrix();
			}
			void SetScale(const glm::vec3& scale) {
				m_Scale = scale;
				UpdateViewMatrix();
			}

			void UpdateViewMatrix() {
				m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Pos) *
					glm::rotate(glm::mat4(1.0f), m_Angle, m_Rot) * 
					glm::scale(glm::mat4(1.0f), m_Scale);

				m_ProjectionViewMatrix = m_ViewMatrix * m_ProjectionMatrix;
			}


			void ShutDown(){}

			glm::vec3& GetPos() { return m_Pos; }
			glm::vec3& GetRot() { return m_Rot; }
			glm::vec3& GetScale() { return m_Scale; }
			float& GetAngle () { return m_Angle; }

		private :
			glm::vec3 m_Pos, m_Rot, m_Scale;
			float m_Angle;

			glm::mat4 m_ViewMatrix;
			glm::mat4 m_ProjectionViewMatrix;
			glm::mat4 m_ProjectionMatrix;
	};


}