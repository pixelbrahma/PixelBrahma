#pragma once

#include "PixelBrahma/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace PixelBrahma
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		// Override graphics context interface functions

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}