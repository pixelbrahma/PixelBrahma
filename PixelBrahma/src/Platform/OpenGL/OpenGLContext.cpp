#include "pbpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace PixelBrahma
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) 
	{
		PB_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	// Initialize OpenGL context
	void OpenGLContext::Init()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Set window context
		glfwMakeContextCurrent(m_WindowHandle);

		// Initialize glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		PB_CORE_ASSERT(status, "Failed to initialize Glad!");

		// Print out OpenGL information
		PB_CORE_INFO("OpenGL Info:");
		PB_CORE_INFO(" Vendor: {0}", glGetString(GL_VENDOR));
		PB_CORE_INFO(" Renderer: {0}", glGetString(GL_RENDERER));
		PB_CORE_INFO(" Version: {0}", glGetString(GL_VERSION));

		PB_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), 
			"PixelBrahma Engine requires at least OpenGL version 4.5!");
	}

	// Swap OpenGL graphics bufffers
	void OpenGLContext::SwapBuffers()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
