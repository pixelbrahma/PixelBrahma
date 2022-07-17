#pragma once

namespace PixelBrahma
{
	// Interface to setup graphics API context 
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}