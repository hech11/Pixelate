#pragma once

#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtx/transform.hpp"

namespace RGF {


	class Camera {

		public :
			Camera()
				: m_View(glm::mat4(1.0f)), m_Projection(glm::mat4(1.0f)), m_Pos({ 0.0f, 0.0f, 0.0f }), m_Rot({1.0f, 1.0f, 1.0f}),
				m_Scale({1.0f, 1.0f, 1.0f}), m_Angle(0.0f) {}

			inline const glm::mat4& GetView() const { return m_View; }
			inline const glm::mat4& GetProjection() const { return m_Projection; }

			void Init(const glm::mat4& view, const glm::mat4& projection) {
				m_View = view;
				m_Projection = projection;
			}

			void SetPosition(glm::vec3 pos) {
				m_Pos = pos;
			}
			void SetRotation(float angle, glm::vec3 axis) {
				m_Rot = axis;
				m_Angle = angle;
			}
			void SetScale(glm::vec3 scale) {
				m_Scale = scale;
			}

			void Update() {
				m_View = glm::translate(glm::mat4(1.0f), m_Pos) * glm::rotate(glm::mat4(1.0f), m_Angle, m_Rot) * glm::scale(glm::mat4(1.0f), m_Scale);

			}
			void ShutDown(){}

			glm::vec3& GetPos() { return m_Pos; }
			glm::vec3& GetRot() { return m_Rot; }
			glm::vec3& GetScale() { return m_Scale; }
			float& GetAngle () { return m_Angle; }

		private :
			glm::vec3 m_Pos, m_Rot, m_Scale;
			float m_Angle;

			glm::mat4 m_View;
			glm::mat4 m_Projection;
	};


}