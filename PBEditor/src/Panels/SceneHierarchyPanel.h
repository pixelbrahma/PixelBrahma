#pragma once

#include "PixelBrahma/Core/Core.h"
#include "PixelBrahma/Log/Log.h"
#include "PixelBrahma/Scene/Scene.h"
#include "PixelBrahma/Scene/Entity.h"

namespace PixelBrahma
{
	// Scene heirarchy panel class
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		// Set scene context function
		void SetContext(const Ref<Scene>& scene);

		// ImGui render function
		void OnImGuiRender();

	private:

		// Draw the entity hierarchy tree function
		void DrawEntityHierarchy(Entity entity);
		// Draw the components of an entity
		void DrawComponentProperties(Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}
