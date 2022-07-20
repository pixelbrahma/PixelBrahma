#pragma once

#include "RenderCommand.h"
#include "Camera/OrthographicCamera.h"
#include "Shader.h"

namespace PixelBrahma
{
	// Renderer class to manage rendering in selected graphics API
	class Renderer
	{
	public:
		
		// Static render queue commands

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, 
			const std::shared_ptr<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f));

		// Static API getter function
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		// Structure containing data relevant to the scene
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}