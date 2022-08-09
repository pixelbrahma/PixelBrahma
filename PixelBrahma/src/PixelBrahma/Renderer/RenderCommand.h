#pragma once

#include "RendererAPI.h"

namespace PixelBrahma
{
	// Command class for static render commands
	class RenderCommand
	{
	public:

		// Ststic renderer initialize function
		static void Init()
		{
			s_RendererAPI->Init();
		}

		// Set viewport size and position
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		//// Static clear commands ////

		// Set the color to clear the screen with
		static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		// Clear the screen buffers
		static void Clear()
		{
			s_RendererAPI->Clear();
		}

		//// Draw calls ///

		// Draw call
		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		// Draw lines function
		static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			s_RendererAPI->DrawLines(vertexArray, vertexCount);
		}

		// Set line width function
		static void SetLineWidth(float width)
		{
			s_RendererAPI->SetLineWidth(width);
		}

	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}
