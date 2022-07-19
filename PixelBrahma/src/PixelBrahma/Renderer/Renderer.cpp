#include "pbpch.h"
#include "Renderer.h"

namespace PixelBrahma
{
	// Set static scene data pointer
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	// Begin scene description
	void Renderer::BeginScene(OrthographicCamera& camera) 
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	// End scene description
	void Renderer::EndScene() {}

	// Submit render commands to the render queue
	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		// Bind shader and set uniform

		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);

		// Bind vertex array and draw object

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}