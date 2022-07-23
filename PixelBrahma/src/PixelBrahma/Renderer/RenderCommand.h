#pragma once

#include "RendererAPI.h"

namespace PixelBrahma
{
	// Command class for static render commands
	class RenderCommand
	{
	public:

		// Ststic renderer initialize function
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		//// Static clear commands ////

		// Set the color to clear the screen with
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		// Clear the screen buffers
		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		//// Draw calls ///

		// Draw call
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}