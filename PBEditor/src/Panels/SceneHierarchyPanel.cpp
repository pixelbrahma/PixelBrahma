#include "SceneHierarchyPanel.h"

#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

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
			DrawEntityHierarchy(entity);
		});

		// Deselect object if clicked elsewhere
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// End ImGui scene hierarchy panel
		ImGui::End();

		// Begin ImGui component properties panel
		ImGui::Begin("Properties");

		// If entity is selected, show its components and their properties
		if (m_SelectionContext)
			DrawComponentProperties(m_SelectionContext);

		// End component properties panel
		ImGui::End();
	}

	// Add entities to the hierarchy tree
	void SceneHierarchyPanel::DrawEntityHierarchy(Entity entity)
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

	// Show the components and their properties for a selected entity
	void SceneHierarchyPanel::DrawComponentProperties(Entity entity)
	{
		// Tag component
		if (entity.HasComponent<TagComponent>())
		{
			// Get the tag component
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			// Tag name buffer
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			// To change tag names
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		// Transform component
		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), 
				ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				// Get the transform component
				auto& transform = entity.GetComponent<TransformComponent>().Transform;

				// Slider for transform slection from  UI
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}
	}
}
