#include "EditorLayer.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include "PixelBrahma/Utils/Math.h"
#include "PixelBrahma/Scene/SceneSerializer.h"
#include "PixelBrahma/Utils/PlatformUtils.h"

namespace PixelBrahma
{
	// Assets directory path
	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true),
		m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f }) {}

	// Layer on attach function
	void EditorLayer::OnAttach()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Load textures from file path

		m_CheckerboardTexture = Texture2D::Create("Assets/Textures/CheckerBoard.png");
		m_IconPlay = Texture2D::Create("Resources/Icons/PlayIcon.png");
		m_IconStop = Texture2D::Create("Resources/Icons/StopIcon.png");

		// Set frame buffer properties
		FramebufferSpecification framebufferSpecification;

		framebufferSpecification.Attachments = 
		{ 
			FramebufferTextureFormat::RGBA8, 
			FramebufferTextureFormat::RED_INTEGER, 
			FramebufferTextureFormat::Depth 
		};

		framebufferSpecification.Width = 1280;
		framebufferSpecification.Height = 720;

		// Create the frambuffer
		m_Framebuffer = Framebuffer::Create(framebufferSpecification);

		// Create scene reference
		m_ActiveScene = CreateRef<Scene>();

		auto commandLineArgs = Application::Get().GetCommandLineArgs();

		// Load file in command line arguments
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(sceneFilePath);
		}

		// Create editor camera
		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0
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
#endif

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
			// Resize the editor camera
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			// Resize the scene camera
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Reset statistics
		Renderer2D::ResetStats();

		// Bind the framebuffer
		m_Framebuffer->Bind();

		// Clear the color buffer
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// Clear the entity ID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);

		switch (m_SceneState)
		{
			case SceneState::Edit:
			{
				if (m_ViewportFocused)
					m_CameraController.OnUpdate(timestep);

				m_EditorCamera.OnUpdate(timestep);

				m_ActiveScene->OnUpdateEditor(timestep, m_EditorCamera);
				break;
			}

			case SceneState::Play:
			{
				m_ActiveScene->OnUpdateRuntime(timestep);
				break;
			}
		}

		// Read pixel under the mouse cursor
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);

			// Get entity under the mouse
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which can't ne undone at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				// New scene menu item with shortcut set
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				// Open scene menu item with shortcut set
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				// Save scene as menu item with shortcut set
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				// Exit menu item with shortcut set
				if (ImGui::MenuItem("Exit"/*, "Alt+F4"*/)) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		// Render the ImGui panels

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		std::string name = "None";

		// If mouse hovering over an entity, display its tag
		if (m_HoveredEntity)
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

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
		
		// Get scene viewport panel bounds
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		// Set viewport size
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		// Get color attachment
		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, 
			ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// Get window size
		auto windowSize = ImGui::GetWindowSize();

		// Drag and drop asset feature
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_AssetPath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos

		// Get selected entity
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, 
				m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Runtime camera
			//auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			//const glm::mat4& cameraProjection = camera.GetProjection();
			//lm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();
	}

	// UI toolbar
	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		// Button colors

		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | 
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		// Image button
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), 
			ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_SceneState == SceneState::Edit)
				OnScenePlay();
			else if (m_SceneState == SceneState::Play)
				OnSceneStop();
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);

		ImGui::End();
	}

	// Layer event handler function
	void EditorLayer::OnEvent(Event& event)
	{
		// Camera event handler
		m_CameraController.OnEvent(event);
		// Editor event handler
		m_EditorCamera.OnEvent(event);

		// Dispatch events
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(PB_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(PB_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	// Handle key events
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		// Shortcuts
		if (event.GetRepeatCount() > 0)
			return false;

		// Get the state of control, shift and alt keys

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		bool alt = Input::IsKeyPressed(Key::LeftAlt) || Input::IsKeyPressed(Key::RightAlt);

		switch (event.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();

				break;
			}

			case Key::O:
			{
				if (control)
					OpenScene();

				break;
			}

			case Key::S:
			{
				if (control && shift)
					SaveSceneAs();
			
				//if (control && !shift)
					//SaveScene();

				break;
			}

			//case Key::F4:
			//{
				//if (alt)
					//ExitScene();
			//}

			// Gizmos
			case Key::Q:
			{			
				if (!ImGuizmo::IsUsing())
					m_GizmoType = -1;
				break;
			}
			case Key::W:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Key::E:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Key::R:
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}
	}

	// Mouse button clicked event handler callback
	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
	{
		if (event.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}

		return false;
	}

	// Create new scene
	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	// Load scene from file
	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogues::OpenFile
			("PixelBrahma Scene (*.PixelBrahma)\0*.PixelBrahma\0");

		if (!filepath.empty())
			OpenScene(filepath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		// Check if the file to load is a scene file
		if (path.extension().string() != ".PixelBrahma")
		{
			PB_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		// Create new scene from file data
		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);

		if (serializer.Deserialize(path.string()))
		{
			m_ActiveScene = newScene;
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		}
	}

	// Save scene as specified
	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogues::SaveFile
			("PixelBrahma Scene (*.PixelBrahma)\0*.PixelBrahma\0");

		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}

	// Scene play function
	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;
		m_ActiveScene->OnRuntimeStart();
	}

	// Scene stop function
	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;
		m_ActiveScene->OnRuntimeStop();
	}
}
