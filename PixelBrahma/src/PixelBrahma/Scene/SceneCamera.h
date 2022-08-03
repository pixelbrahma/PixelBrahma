#pragma once

#include "PixelBrahma/Renderer/Camera/Camera.h"

namespace PixelBrahma
{
	// The scene camera class
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		// Set orthographic projection
		void SetOrthographic(float size, float nearClip, float farClip);

		// Set viewpert size
		void SetViewportSize(uint32_t width, uint32_t height);

		// Orthographic size getter and setter functions

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }

	private:
		// Recalculate orthographic projection function
		void RecalculateProjection();

	private:
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;
		float m_AspectRatio = 0.0f;
	};
}