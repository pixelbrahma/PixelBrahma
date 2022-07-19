#include "pbpch.h"
#include "Renderer.h"

namespace PixelBrahma
{
	// Begin scene description
	void Renderer::BeginScene() {}

	// End scene description
	void Renderer::EndScene() {}

	// Submit render commands to the render queue
	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}