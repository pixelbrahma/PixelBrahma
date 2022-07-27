#pragma once

#include "RenderCommand.h"
#include "Camera/OrthographicCamera.h"
#include "Shading/Shader.h"

namespace PixelBrahma
{
	// Renderer class to manage rendering in selected graphics API
	class Renderer
	{
	public:

		// Static initialize and shutdown functions

		static void Init();
		static void Shutdown();

		// Window resize event handler
		static void OnWindowResize(uint32_t width, uint32_t height);
		
		// Static render queue commands

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, 
			const Ref<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f));

		// Static API getter function
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		// Structure containing data relevant to the scene
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};
}