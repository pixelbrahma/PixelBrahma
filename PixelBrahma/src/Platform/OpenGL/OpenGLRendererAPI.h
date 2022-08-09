#pragma once

#include "PixelBrahma/Renderer/RendererAPI.h"

namespace PixelBrahma
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:

		// Initialize renderer function override
		virtual void Init() override;

		// Set viewport size and position function override
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		// Clear functions override

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		// Draw functions override

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		
		// Set line width function override
		virtual void SetLineWidth(float width) override;
	};
}
