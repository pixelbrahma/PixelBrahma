#include "pbpch.h"
#include "Renderer.h"

#include "Renderer2D.h"

namespace PixelBrahma
{
	// Set static scene data pointer
	Scope<Renderer::SceneData> Renderer::s_SceneData =  CreateScope<Renderer::SceneData>();

	// Initialize renderer function
	void Renderer::Init()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	// Shutdown function
	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	// Window resize event handler
	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		// Set viewport size to match the resized window
		RenderCommand::SetViewport(0, 0, width, height);
	}

	// Begin scene description
	void Renderer::BeginScene(OrthographicCamera& camera) 
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	// End scene description
	void Renderer::EndScene() {}

	// Submit render commands to the render queue
	void Renderer::Submit(const Ref<Shader>& shader, 
		const Ref<VertexArray>& vertexArray,
		const glm::mat4& transform)
	{
		// Bind shader and set uniform

		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		// Bind vertex array and draw object

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}