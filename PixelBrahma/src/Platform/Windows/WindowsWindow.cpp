#include "pbpch.h"
#include "WindowsWindow.h"

namespace PixelBrahma
{
	// GLFW initialized flag - Even if multiple windows are created, GLFW needs to be initialized only once
	static bool s_GLFWInitialized = false;

	// Create window function
	Window* Window::Create(const WindowProps& props) { return new WindowsWindow(props); }

	WindowsWindow::WindowsWindow(const WindowProps& props) { Init(props); }

	WindowsWindow::~WindowsWindow() { Shutdown(); }

	// Initialize window function
	void WindowsWindow::Init(const WindowProps& props)
	{
		// Set properties

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		PB_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		// Initialize GLFW 

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			PB_CORE_ASSERT(success, "Could not initialize GLFW!");

			s_GLFWInitialized = true;
		}

		// Create GLFW window and set context

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	// Window shutdown function
	void WindowsWindow::Shutdown() { glfwDestroyWindow(m_Window); }

	// Window update function
	void WindowsWindow::OnUpdate()
	{
		// Poll events and swap buffers

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	// Set Vsync
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	// Return Vsync status
	bool WindowsWindow::IsVSync() const { return m_Data.VSync; }
}