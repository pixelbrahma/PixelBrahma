#pragma once

namespace PixelBrahma
{
	// Interface to setup graphics API context 
	class GraphicsContext
	{
	public:

		// Initialize context
		virtual void Init() = 0;

		// Swap buffers
		virtual void SwapBuffers() = 0;

		// Create context
		static Scope<GraphicsContext> Create(void* window);
	};
}