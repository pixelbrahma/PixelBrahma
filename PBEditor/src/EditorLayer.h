#pragma once

#include <PixelBrahma.h>

#include "Panels/SceneHierarchyPanel.h"

namespace PixelBrahma
{
	// Editor layer class
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		// Layer attach and detach overrides

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		// Update function override
		virtual void OnUpdate(Timestep timestep) override;

		// ImGui render function override
		virtual void OnImGuiRender() override;

		// Event handling function override
		virtual void OnEvent(Event& event) override;

	private:

		// On key pressed event function
		bool OnKeyPressed(KeyPressedEvent& event);

		// Scene saving, loading and creating file menu option functions
		
		void NewScene();
		void OpenScene();
		void SaveSceneAs();
		//void ExitScene();
		//void SaveScene();

	private:
		OrthographicCameraController m_CameraController;

		// Temporary tests
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;
		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}
