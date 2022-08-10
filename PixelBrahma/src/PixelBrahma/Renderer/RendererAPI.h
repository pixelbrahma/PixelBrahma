#pragma once

#include "PixelBrahma/Renderer/Geometry/VertexArray.h"

#include <glm/glm.hpp>

namespace PixelBrahma
{
	// Renderer API interface
	class RendererAPI
	{
	public:
		// Enumeration class of graphics rendering API's
		enum class API
		{
			None = 0, OpenGL = 1
		};

	public:
		virtual ~RendererAPI() = default;

		// Virtual renderer initialize function
		virtual void Init() = 0;

		// Set viewport size and position
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		// Clear functions

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		// Draw functions

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

		// Set line width function
		virtual void SetLineWidth(float width) = 0;

		// Static API getter function
		static API GetAPI() { return s_API; }

		// Create RendererAPI
		static Scope<RendererAPI> Create();

	private:
		static API s_API;
	};
}
