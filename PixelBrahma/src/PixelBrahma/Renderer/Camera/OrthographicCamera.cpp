#include "pbpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace PixelBrahma
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) :
		m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	// Set camera projection function
	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	// Recalculate camera matrices
	void OrthographicCamera::RecalculateViewMatrix()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Translation * rotation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		// Invert the view matrix
		m_ViewMatrix = glm::inverse(transform);

		// Calculate view projection matrix
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}