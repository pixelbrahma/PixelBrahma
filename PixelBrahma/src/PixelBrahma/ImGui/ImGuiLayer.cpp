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
		// Profiling
		PB_PROFILE_FUNCTION();

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
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Bold font
		io.Fonts->AddFontFromFileTTF("Assets/Fonts/Rubik/Rubik-Bold.ttf", 18.0f);
		// Default font
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Rubik/Rubik-Regular.ttf", 18.0f);

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

		// Set the theme to dark colors
		SetDarkThemeColors();

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
		// Profiling
		PB_PROFILE_FUNCTION();

		// Clean up ImGui resources

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	// ImGui layer event handler
	void ImGuiLayer::OnEvent(Event& event)
	{
		// If events need to be blocked
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	// ImGui begin function
	void ImGuiLayer::Begin()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// New ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	// ImGui end function
	void ImGuiLayer::End()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

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

	// Dark theme
	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg]                 = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
									        
		// Headers					        
		colors[ImGuiCol_Header]                   = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered]            = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive]             = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
									              
		// Buttons					              
		colors[ImGuiCol_Button]                   = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered]            = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive]             = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
										          
		// Frame BG						          
		colors[ImGuiCol_FrameBg]                  = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered]           = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive]            = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
										          
		// Tabs							          
		colors[ImGuiCol_Tab]                      = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered]               = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive]                = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused]             = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive]       = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
											      
		// Title							      
		colors[ImGuiCol_TitleBg]                  = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive]            = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed]         = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
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
