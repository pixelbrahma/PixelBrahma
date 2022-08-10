#pragma once

#include "Camera.h"

#include "PixelBrahma/Core/Timestep.h"

#include "PixelBrahma/Events/Event.h"
#include "PixelBrahma/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace PixelBrahma
{
	// The editor camera class
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		// Update and event handling functions

		void OnUpdate(Timestep ts);
		void OnEvent(Event& event);

		// Camera distance getter and setter functions 

		float GetDistance() const { return m_Distance; }
		void SetDistance(float distance) { m_Distance = distance; }

		// Set viewport size function
		void SetViewportSize(float width, float height) 
		{ 
			// Set sizes and update projection matrix
			m_ViewportWidth = width; 
			m_ViewportHeight = height; 
			UpdateProjection(); 
		}

		// Camera matrices getter functions

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		// Camera position and orientation getter functions

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		// Camera pitch and yaw getter functions

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

	private:

		// Update projection  and view matrices function
		void UpdateProjection();
		void UpdateView();

		// Mouse input handling functions

		bool OnMouseScroll(MouseScrolledEvent& event);
		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		// Calculate camera position function
		glm::vec3 CalculatePosition() const;

		// Camera movement speed functions

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:
		float m_FOV = 45.0f;
		float m_AspectRatio = 1.778f;
		float m_NearClip = 0.1f;
		float m_FarClip = 1000.0f;
		float m_Distance = 10.0f;
		float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;
		float m_ViewportWidth = 1280;
		float m_ViewportHeight = 720;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };
	};
}
