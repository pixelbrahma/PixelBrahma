#include "pbpch.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace PixelBrahma
{
	// Initialize renderer function
	void OpenGLRendererAPI::Init()
	{
		// Enable blending and setup blending function
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	// Set the color to clear the screen with
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	// Clear screen buffers
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// Draw call
	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}