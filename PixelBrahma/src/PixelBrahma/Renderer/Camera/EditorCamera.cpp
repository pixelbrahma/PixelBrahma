#include "pbpch.h"
#include "EditorCamera.h"

#include "PixelBrahma/Input/Input.h"
#include "PixelBrahma/Input/KeyCodes.h"
#include "PixelBrahma/Input/MouseCodes.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace PixelBrahma
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip),
		Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		// Update view matrix
		UpdateView();
	}

	// Update the camera projection matrix
	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
	}

	// Update the view matrix
	void EditorCamera::UpdateView()
	{
		// Lock the camera's rotation
		// m_Yaw = m_Pitch = 0.0f; 

		// Get position
		m_Position = CalculatePosition();
		// Get orientation
		glm::quat orientation = GetOrientation();

		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	// Camera pan speed
	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		// Pan x-axis
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
		// Pan y-axis
		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	// Camera rotation speed
	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	// Camera zoom speed
	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	// Editor camera update 
	void EditorCamera::OnUpdate(Timestep timestep)
	{
		// Pan - alt + middle mouse button
		// Rotate - alt + left mouse button
		// Zoom - alt + Right mouse button
		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			// Get mouse position
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);
		}

		// Update view matrix
		UpdateView();
	}

	// Editor camera event handler 
	void EditorCamera::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(PB_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	// Mouse scroll event handler 
	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& event)
	{
		float delta = event.GetYOffset() * 0.1f;
		MouseZoom(delta);

		// Update view matrix
		UpdateView();

		return false;
	}

	// Pan using mouse 
	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	// Rotate using mouse function
	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	// Zoom using mouse function
	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	// Get camera up direction
	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	// Get camera right direction
	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	// Get camera forward direction
	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	// Calculate camera position
	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	// Get camera orientation
	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}
}
