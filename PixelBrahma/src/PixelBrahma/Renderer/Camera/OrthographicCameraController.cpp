#include "pbpch.h"
#include "OrthographicCameraController.h"

#include "PixelBrahma/Input/Input.h"
#include "PixelBrahma/Input/KeyCodes.h"

namespace PixelBrahma
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio * m_ZoomLevel,     // Left
			m_AspectRatio* m_ZoomLevel,            // Right
			-m_ZoomLevel,                          // Bottom
			m_ZoomLevel),                          // Top
		m_Rotation(rotation) {}

	// Camera update function
	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		// Translation movement

		if (Input::IsKeyPressed(PB_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * timestep;
		else if (Input::IsKeyPressed(PB_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * timestep;

		if (Input::IsKeyPressed(PB_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * timestep;
		else if (Input::IsKeyPressed(PB_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * timestep;

		// Rotation movement

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(PB_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * timestep;
			else if (Input::IsKeyPressed(PB_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * timestep;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		// Set camera movement speed to be dependant on the zoom level
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	// Event handling function
	void OrthographicCameraController::OnEvent(Event& event)
	{
		// Dispatch the event with the event dispatcher
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(PB_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(PB_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	// Mouse scrolled event handling
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		// Set zoom level
		m_ZoomLevel -= event.GetYOffset() * 0.25f;

		// Clamp max zoom level
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		
		// Set camera projection
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	// Window resize event handling
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		// Set aspect ratio
		m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		
		// Set camera projection
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}
}