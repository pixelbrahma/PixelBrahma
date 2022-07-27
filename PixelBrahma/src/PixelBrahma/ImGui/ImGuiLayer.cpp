#include "pbpch.h"
#include "ImGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "PixelBrahma/Application/Application.h"

// Temporary
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace PixelBrahma
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

	// Functionality when the layer is added to the layer stack
	void ImGuiLayer::OnAttach()
	{
		// ImGui context and style
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Get InGui input output reference
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Enable keyboard controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		// Enable gamepad controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		// Enable docking
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Enable multi-viewport / platform windows
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Set ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows 
		//can look identical to regular ones.

		ImGuiStyle& style = ImGui::GetStyle();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Get application and GLFW window reference

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup platform/renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);

		// Initialize ImGui opengl
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	// Functionality when the layer is removed from the layer stack
	void ImGuiLayer::OnDetach() 
	{
		// Clean up ImGui resources

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	// ImGui begin function
	void ImGuiLayer::Begin()
	{
		// New ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	// ImGui end function
	void ImGuiLayer::End()
	{
		// Get References to ImGui i/o and application

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();

		// Set ImGui display size to the window size
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
/*
	// ImGui show function
	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}
*/
}