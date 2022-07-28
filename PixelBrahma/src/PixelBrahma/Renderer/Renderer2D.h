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

		// Draw Quads

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
			float tilingFactor = 1.0f, const glm::vec4& tiniColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
			float tilingFactor = 1.0f, const glm::vec4& tiniColor = glm::vec4(1.0f));

		// Draw rotated quads

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, 
			const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, 
			const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, 
			const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, 
			const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

	};

}