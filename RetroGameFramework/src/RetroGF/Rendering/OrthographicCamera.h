#pragma once

#include <GLM/glm/glm.hpp>
#include "GLM/glm/gtx/transform.hpp"


namespace RGF {


	class OrthographicCamera {

	public:

		OrthographicCamera(float left, float right, float bottom, float top);

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ProjectionViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

		void SetPosition(const glm::vec3& pos);
		void Move(const glm::vec3& offset);

		void SetRotation(float angle, float axis);

		void SetScale(const glm::vec3& scale);
		void Enlarge(const glm::vec3& offset);



		glm::vec3& GetPos() { return m_Pos; }
		float& GetRot() { return m_RotAxis; }
		glm::vec3& GetScale() { return m_Scale; }
		float& GetAngle() { return m_Angle; }

	private:
		void RecalculateViewProjMatrix();

	private:
		glm::vec3 m_Pos, m_Scale;
		float m_Angle, m_RotAxis;

		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionViewMatrix;
		glm::mat4 m_ProjectionMatrix;
	};


}