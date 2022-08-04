#include "SceneHierarchyPanel.h"

#include <ImGui/imgui.h>

#include "PixelBrahma/Scene/Components.h"

namespace PixelBrahma
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	// Set the scene context
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	// Imgui rendering
	void SceneHierarchyPanel::OnImGuiRender()
	{
		// Begin ImGui scene hierarchy panel 
		ImGui::Begin("Scene Hierarchy");

		// Get each entity in the scene registry
		m_Context->m_Registry.each([&](auto entityID)
		{
			// Create an engine entity instance of it
			Entity entity(entityID, m_Context.get());
			// Add to the entity hierarchy tree
			DrawEntityNode(entity);
		});

		// End ImGui scene hierarchy panel
		ImGui::End();
	}

	// Add entities to the hierarchy tree
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		// Get entity tag
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) 
			| ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}
}
