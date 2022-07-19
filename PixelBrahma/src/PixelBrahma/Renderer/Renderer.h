#pragma once

#include "RenderCommand.h"

namespace PixelBrahma
{
	// Renderer class to manage rendering in selected graphics API
	class Renderer
	{
	public:
		
		// Static render queue commands

		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		// Static API getter function
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}