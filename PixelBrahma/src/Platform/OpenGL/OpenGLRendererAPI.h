#pragma once

#include "PixelBrahma/Renderer/RendererAPI.h"

namespace PixelBrahma
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:

		// Clear functions override

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		// Draw function override
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}