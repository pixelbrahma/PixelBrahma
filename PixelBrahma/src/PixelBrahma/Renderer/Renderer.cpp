#include "pbpch.h"
#include "Renderer.h"
#include "Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace PixelBrahma
{
	// Set static scene data pointer
	Scope<Renderer::SceneData> Renderer::s_SceneData =  CreateScope<Renderer::SceneData>();

	// Initialize renderer function
	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
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
	void Renderer::Submit(const std::shared_ptr<Shader>& shader, 
		const std::shared_ptr<VertexArray>& vertexArray,
		const glm::mat4& transform)
	{
		// Bind shader and set uniform

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection",
			s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		// Bind vertex array and draw object

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}