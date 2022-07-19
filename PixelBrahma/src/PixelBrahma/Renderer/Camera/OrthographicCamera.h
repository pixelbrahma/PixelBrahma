#pragma once

#include "glm/glm.hpp"

namespace PixelBrahma
{
	// 2D orthographic camera class
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		// Position getter and setter functions

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		// Rotation getter and setter functions

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		// Camera matrices getter functions

		const glm::mat4& GetProjectionMatrix() const     { return m_ProjectionMatrix;     }
		const glm::mat4& GetViewMatrix() const           { return m_ViewMatrix;           }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:

		// Recalculate camera matrices 
		void RecalculateViewMatrix();

	private:

		// Camera matrices 

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		// Camera transforms

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}