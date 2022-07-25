#pragma once

#include "Camera/OrthographicCamera.h"
#include "Shading/Texture.h"

namespace PixelBrahma
{
	// 2D renderer class for designated 2D rendering
	class Renderer2D
	{
	public:
		
		// Initialize and shutdown functions
		
		static void Init();
		static void Shutdown();

		// Begin and end scene functions

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Draw primitives functions

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
	};
}