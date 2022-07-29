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

		#ifdef PB_ENABLE_ASSERTS
				int versionMajor;
				int versionMinor;
				glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
				glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

				PB_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), 
					"PixelBrahma engine requires at least OpenGL version 4.5!");
		#endif
	}

	// Swap OpenGL graphics bufffers
	void OpenGLContext::SwapBuffers()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}