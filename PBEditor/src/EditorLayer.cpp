#include "EditorLayer.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace PixelBrahma
{
	EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true),
		m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f }) {}

	// Layer on attach function
	void EditorLayer::OnAttach()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Load texture from file path
		m_CheckerboardTexture = Texture2D::Create("Assets/Textures/CheckerBoard.png");

		// Set frame buffer properties
		FramebufferSpecification framebufferSpecification;
		framebufferSpecification.Width = 1280;
		framebufferSpecification.Height = 720;

		// Create the frambuffer
		m_Framebuffer = Framebuffer::Create(framebufferSpecification);

		// Create scene reference
		m_ActiveScene = CreateRef<Scene>();

		// Green square

		// Create square entity
		auto greenSquare = m_ActiveScene->CreateEntity("Green Square");
		// Add a sprite renderer component to the square
		greenSquare.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		m_SquareEntity = greenSquare;

		// Red square

		// Create square entity
		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		// Add a sprite renderer component to the square and move it to the right
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		redSquare.GetComponent<TransformComponent>().Translation.x = 2.0f;

		// Create a camera entity and add a camera component
		m_CameraEntity = m_ActiveScene->CreateEntity("Main Camera");
		m_CameraEntity.AddComponent<CameraComponent>();

		// Create the secondary camera entity and add camera component
		m_SecondCamera = m_ActiveScene->CreateEntity("Alternate Camera");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		// Camera controller class which is scriptable
		class CameraController : public ScriptableEntity
		{
		public:
			// Create function
			virtual void OnCreate() override
			{
				// Get the transform component
				auto& translation = GetComponent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 5.0f;
			}

			// Destroy function
			virtual void OnDestroy() override {}

			// Update function
			virtual void OnUpdate(Timestep timestep) override
			{
				// Get the transform component
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				// Key bindings

				if (Input::IsKeyPressed(Key::A))
					translation.x -= speed * timestep;
				if (Input::IsKeyPressed(Key::D))
					translation.x += speed * timestep;
				if (Input::IsKeyPressed(Key::W))
					translation.y += speed * timestep;
				if (Input::IsKeyPressed(Key::S))
					translation.y -= speed * timestep;
			}
		};

		// Add camera controller script component to the camera entities
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		// Set the context of the scene hierarchy panel
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	// Layer on detach function
	void EditorLayer::OnDetach()
	{
		// Profiling
		PB_PROFILE_FUNCTION();
	}

	// Layer update function
	void EditorLayer::OnUpdate(Timestep timestep)
	{
		// Sandbox update function profiling
		PB_PROFILE_FUNCTION();

		// Resize
		if (FramebufferSpecification specification = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(specification.Width != m_ViewportSize.x || specification.Height != m_ViewportSize.y))
		{
			// Resize the framebuffer to the new viewport size
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			// Resize the camera to the new viewport size
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			// Resize the scene camera
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// If the viewport panel is in focus
		if (m_ViewportFocused)
		{
			// Call camera update function
			m_CameraController.OnUpdate(timestep);
		}

		// Reset statistics
		Renderer2D::ResetStats();

		// Bind the framebuffer
		m_Framebuffer->Bind();

		// Clear the color buffer
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// Update scene entities
		m_ActiveScene->OnUpdate(timestep);

		// Unbind the framebuffer
		m_Framebuffer->UnBind();
	}

	// ImGui render function
	void EditorLayer::OnImGuiRender()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Docking - Dockspace

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistent = true;
		bool opt_fullscreen = opt_fullscreen_persistent;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// Using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render 
		// the background and handle the pass-thru hole, so Begin() is told to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: Proceed even if Begin() returns false (aka window is collapsed).
		// This is to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// Cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dock space
		ImGuiIO& io = ImGui::GetIO();

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which can't ne undone at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		// Render the scene hierarchy panel
		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		// Stats
		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		// Viewport 
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		// Set viewport size
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		// Get color attachment
		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, 
			ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	// Layer event handler function
	void EditorLayer::OnEvent(Event& event)
	{
		m_CameraController.OnEvent(event);
	}
}
