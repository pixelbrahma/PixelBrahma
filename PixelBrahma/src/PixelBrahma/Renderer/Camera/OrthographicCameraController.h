#pragma once

#include "PixelBrahma/Renderer/Camera/OrthographicCamera.h"
#include "PixelBrahma/Core/Timestep.h"

#include "PixelBrahma/Events/ApplicationEvent.h"
#include "PixelBrahma/Events/MouseEvent.h"

namespace PixelBrahma
{
	// 2D camera controller wrapper class for othrographic camera
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& e);

		// Get camera functions

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:

		// Event handler functions

		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;

		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;
	};
}