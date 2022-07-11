#include "pbpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "GLFW/glfw3.h"

#include "PixelBrahma/Application.h"

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
	void ImGuiLayer::OnEvent(Event& e) {}
}