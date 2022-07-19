#pragma once

#include "glm/glm.hpp"

#include "VertexArray.h"

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

		// Clear functions

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		// Draw function
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		// Static API getter function
		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}