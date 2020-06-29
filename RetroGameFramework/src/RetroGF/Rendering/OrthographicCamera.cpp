#include "RGFpch.h"
#include "OrthographicCamera.h"

#include "RetroGF/Debug/Instrumentor.h"



namespace RGF {


	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_Pos({ 0.0f, 0.0f, 0.0f }), m_RotAxis(1.0f),
		m_Scale({ 1.0f, 1.0f, 1.0f }), m_Angle(0.0f), m_ViewMatrix(glm::mat4(1.0f)), m_ProjectionViewMatrix(1.0f), m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))

	{

		RGF_PROFILE_FUNCTION();
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
		RGF_PROFILE_FUNCTION();
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}


	void OrthographicCamera::SetPosition(const glm::vec3& pos) {
		RGF_PROFILE_FUNCTION();
		m_Pos = pos;
		RecalculateViewProjMatrix();
	}

	void OrthographicCamera::Move(const glm::vec3& offset) {
		RGF_PROFILE_FUNCTION();
		m_Pos += offset;
		RecalculateViewProjMatrix();
	}

	void OrthographicCamera::SetRotation(float angle, float axis) {
		RGF_PROFILE_FUNCTION();
		m_RotAxis = axis;
		m_Angle = angle;
		RecalculateViewProjMatrix();
	}
	void OrthographicCamera::SetScale(const glm::vec3& scale) {
		RGF_PROFILE_FUNCTION();
		m_Scale = scale;
		RecalculateViewProjMatrix();
	}


	void OrthographicCamera::Enlarge(const glm::vec3& offset) {
		RGF_PROFILE_FUNCTION();
		m_Scale += offset;
		RecalculateViewProjMatrix();
	}

	void OrthographicCamera::RecalculateViewProjMatrix() {
		RGF_PROFILE_FUNCTION();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Pos) *
			glm::rotate(glm::mat4(1.0f), m_Angle, {0.0f, 0.0f, m_RotAxis }) *
			glm::scale(glm::mat4(1.0f), m_Scale);

		m_ViewMatrix = glm::inverse(m_ViewMatrix);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}



}
