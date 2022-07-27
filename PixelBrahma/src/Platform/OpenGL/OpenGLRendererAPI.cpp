#include "pbpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace PixelBrahma
{
	// Initialize renderer function
	void OpenGLRendererAPI::Init()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Enable blending and setup blending function
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
	}

	// Set viewport size and position function
	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
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
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

		// Unbind texture after drawing
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}