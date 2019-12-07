#include "RGFpch.h"
#include "OrthographicCamera.h"




namespace RGF {


	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_Pos({ 0.0f, 0.0f, 0.0f }), m_RotAxis(1.0f),
		m_Scale({ 1.0f, 1.0f, 1.0f }), m_Angle(0.0f), m_ViewMatrix(glm::mat4(1.0f)), m_ProjectionViewMatrix(1.0f), m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))

	{

		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


	void OrthographicCamera::SetPosition(const glm::vec3& pos) {
		m_Pos = pos;
		RecalculateViewProjMatrix();
	}

	void OrthographicCamera::Move(const glm::vec3& offset) {
		m_Pos += offset;
		RecalculateViewProjMatrix();
	}

	void OrthographicCamera::SetRotation(float angle, float axis) {
		m_RotAxis = axis;
		m_Angle = angle;
		RecalculateViewProjMatrix();
	}
	void OrthographicCamera::SetScale(const glm::vec3& scale) {
		m_Scale = scale;
		RecalculateViewProjMatrix();
	}


	void OrthographicCamera::Enlarge(const glm::vec3& offset) {
		m_Scale += offset;
		RecalculateViewProjMatrix();
	}

	void OrthographicCamera::RecalculateViewProjMatrix() {
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Pos) *
			glm::rotate(glm::mat4(1.0f), m_Angle, {0.0f, 0.0f, m_RotAxis }) *
			glm::scale(glm::mat4(1.0f), m_Scale);

		m_ViewMatrix = glm::inverse(m_ViewMatrix);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}



}
