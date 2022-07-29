#include "pbpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace PixelBrahma
{
	// OpenGL message logging function
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         PB_CORE_CRITICAL(message);   return;
			case GL_DEBUG_SEVERITY_MEDIUM:       PB_CORE_ERROR(message);      return;
			case GL_DEBUG_SEVERITY_LOW:          PB_CORE_WARN(message);       return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: PB_CORE_TRACE(message);      return;
		}

		PB_CORE_ASSERT(false, "Unknown severity level!");
	}

	// Initialize renderer function
	void OpenGLRendererAPI::Init()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

	// Enable OpenGL message logging callback if in debug mode
	#ifdef PB_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif

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
	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count)
	{
		uint32_t indexCount = count ? count : vertexArray->GetIndexBuffer()->GetCount();

		// OpenGL draw call
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

		// Unbind texture after drawing
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}