#include "pbpch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "GL/GL.h"

namespace PixelBrahma
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) 
	{
		PB_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	// Initialize OpenGL context
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PB_CORE_ASSERT(status, "Failed to initialize Glad!");
	}

	// Swap OpenGL graphics bufffers
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}