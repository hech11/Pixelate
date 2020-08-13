#include "RGFpch.h"
#include "OrthographicCamera.h"

#include "RetroGF/Debug/Instrumentor.h"



namespace RGF {


	OrthographicCamera::OrthographicCamera(const OrthographicCameraBounds& bounds, float orthoSize)
		: m_OrthographicSize(orthoSize), m_AspectRatio(0.0f), m_Angle(0.0f), m_Pos({ 0.0f, 0.0f, 0.0f }), m_Bounds(bounds),
		m_ViewMatrix(glm::mat4(1.0f)), m_ProjectionViewMatrix(1.0f), 
		m_ProjectionMatrix(glm::ortho(bounds.Left, bounds.Right, bounds.Bottom, bounds.Top, -1.0f, 1.0f))

	{

		RGF_PROFILE_FUNCTION();
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
		RGF_PROFILE_FUNCTION();
		m_Bounds = {left, right, bottom, top};

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}


	void OrthographicCamera::SetProjection(const OrthographicCameraBounds& bounds) {
		SetProjection(bounds.Left, bounds.Right, bounds.Bottom, bounds.Top);
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

	void OrthographicCamera::SetRotation(float angle) {
		RGF_PROFILE_FUNCTION();
		m_Angle = angle;
		RecalculateViewProjMatrix();
	}
	

	void OrthographicCamera::Resize(float aspectRatio) {
		m_AspectRatio = aspectRatio;
		m_Bounds = { -m_AspectRatio * m_OrthographicSize, m_AspectRatio * m_OrthographicSize, -m_OrthographicSize, m_OrthographicSize };
		SetProjection(m_Bounds);
	}

	void OrthographicCamera::SetViewMatrix(const glm::mat4& matrix) {
		m_ViewMatrix = matrix;
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


	void OrthographicCamera::SetOrthographicSize(float size) {
		m_OrthographicSize = size;
		Resize(m_AspectRatio);
	}

	void OrthographicCamera::RecalculateViewProjMatrix() {
		RGF_PROFILE_FUNCTION();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Pos) *
			glm::rotate(glm::mat4(1.0f), m_Angle, { 0.0f, 0.0f, 1.0f });

		m_ViewMatrix = glm::inverse(m_ViewMatrix);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}



}
