#include "pbpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "PixelBrahma/Application/Application.h"

// Temporary
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace PixelBrahma
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}
	ImGuiLayer::~ImGuiLayer() {}

	// Functionality when the layer is added to the layer stack
	void ImGuiLayer::OnAttach()
	{
		// ImGui context and style
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		// ImGui Io reference and backend flags
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY: should eventually use our own key codes
		io.KeyMap[ImGuiKey_Tab]         = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow]   = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow]  = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow]     = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow]   = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp]      = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown]    = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home]        = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End]         = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert]      = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete]      = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace]   = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space]       = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter]       = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape]      = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A]           = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C]           = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V]           = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X]           = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y]           = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z]           = GLFW_KEY_Z;

		// Initialize ImGui opengl
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	// Functionality when the layer is removed from the layer stack
	void ImGuiLayer::OnDetach() {}

	// ImGui update function
	void ImGuiLayer::OnUpdate()
	{
		// Get ImGui and Application references

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		// Calculate delta time and update the same

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		// New ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		// Show ImGui window
		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		// Render ImGui data
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	// ImGui event handling
	void ImGuiLayer::OnEvent(Event& event) 
	{
		// Create dispatcher
		EventDispatcher dispatcher(event);

		// Dispatch mouse button events
		dispatcher.Dispatch<MouseButtonPressedEvent>(PB_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(PB_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));

		// Dispatch mouse move events
		dispatcher.Dispatch<MouseMovedEvent>(PB_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(PB_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));

		// Dispatch key events
		dispatcher.Dispatch<KeyPressedEvent>(PB_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(PB_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(PB_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));

		// Dispatch window events
		dispatcher.Dispatch<WindowResizeEvent>(PB_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}

	// Mouse button pressed event handling
	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}

	// Mouse button released event handling
	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}

	// Mouse moved event handling
	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	// Mouse scrolled event handling
	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}

	// Key pressed event handling
	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		// Keys which have 2 key codes - Modifier keys

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		return false;
	}

	// Key pressed event handling
	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	// Key typed event handling
	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();

		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	// Window resize event handling
	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}
}