#include "pbpch.h"
#include "WindowsWindow.h"

#include "PixelBrahma/Events/ApplicationEvent.h"
#include "PixelBrahma/Events/KeyEvent.h"
#include "PixelBrahma/Events/MouseEvent.h"
#include "PixelBrahma/Renderer/Renderer.h"
#include "PixelBrahma/Input/Input.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace PixelBrahma
{
	// GLFW initialized count - Even if multiple windows are created, GLFW needs to be initialized only once
	static uint32_t s_GLFWWindowCount = 0;

	// Callback function for GLFW errors
	static void GLFWErrorCallback(int error, const char* description)
	{
		PB_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	// Create window function
	Scope<Window> Window::Create(const WindowProps& props) { return CreateScope<WindowsWindow>(props); }

	WindowsWindow::WindowsWindow(const WindowProps& props) 
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		Init(props); 
	}

	WindowsWindow::~WindowsWindow() 
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		Shutdown(); 
	}

	// Initialize window function
	void WindowsWindow::Init(const WindowProps& props)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Set properties

		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		PB_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		// Initialize GLFW if no GLFW windows already exist

		if (s_GLFWWindowCount == 0)
		{
			PB_PROFILE_SCOPE("glfwInit");

			int success = glfwInit();
			PB_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			PB_PROFILE_SCOPE("glfwCreateWindow");

		#if defined(PB_DEBUG)
					if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
						glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif

			// Create GLFW window
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		// Create OpenGL context and initialize it
		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks

		// Window resize event
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				// Get the user pointer which points to WindowData structure type and set width and height
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				// Create a window resize event and dispatch it
				WindowResizeEvent event(width, height);	
				data.EventCallback(event);
			});

		// Window close event
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				// Get the user pointer which points to WindowData structure type and set width and height
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				// Create a window close event and dispatch it
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		// Key event
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				// Get the user pointer which points to WindowData structure type and set width and height
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						// Create a key pressed event and dispatch it
						KeyPressedEvent event(static_cast<KeyCode>(key), 0);
						data.EventCallback(event);

						break;
					}

					case GLFW_RELEASE:
					{
						// Create a key released event and dispatch it
						KeyReleasedEvent event(static_cast<KeyCode>(key));
						data.EventCallback(event);

						break;
					}

					case GLFW_REPEAT:
					{
						// Create a key pressed event and dispatch it
						// GLFW doesnt provide repeat count but win32 api does
						KeyPressedEvent event(static_cast<KeyCode>(key), 1);		
						data.EventCallback(event);

						break;
					}
				}
			});

		// Key typed event
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				// Get the user pointer which points to WindowData structure type and set width and height
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(static_cast<KeyCode>(keycode));
				data.EventCallback(event);
			});

		// Mouse button event
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				// Get the user pointer which points to WindowData structure type and set width and height
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						// Create a mouse button pressed event and dispatch it
						MouseButtonPressedEvent event(static_cast<MouseCode>(button));
						data.EventCallback(event);

						break;
					}

					case GLFW_RELEASE:
					{
						// Create a mouse button released event and dispatch it
						MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
						data.EventCallback(event);

						break;
					}
				}
			});

		// Mouse scroll event
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				// Get the user pointer which points to WindowData structure type and set width and height
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				// Create a mouse scroll event and dispatch it
				MouseScrolledEvent event((float)xoffset, (float)yoffset);
				data.EventCallback(event);
			});

		// Mouse position event
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
			{
				// Get the user pointer which points to WindowData structure type and set width and height
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				// Create a mouse moved event and dispacth it
				MouseMovedEvent event((float)xpos, (float)ypos);
				data.EventCallback(event);
			});
	}

	// Window shutdown function
	void WindowsWindow::Shutdown() 
	{ 
		// Profiling
		PB_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window); 
		--s_GLFWWindowCount;

		// If no GLFW windows are open
		if (s_GLFWWindowCount == 0) { glfwTerminate(); }
	}

	// Window update function
	void WindowsWindow::OnUpdate()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Poll events and swap buffers

		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	// Set Vsync
	void WindowsWindow::SetVSync(bool enabled)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	// Return Vsync status
	bool WindowsWindow::IsVSync() const { return m_Data.VSync; }
}