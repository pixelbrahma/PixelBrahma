#pragma once

#include <entt.hpp>

#include "PixelBrahma/Core/Timestep.h"

namespace PixelBrahma
{
	// Forward declaration of the entity class
	class Entity;

	// Scene management class
	class Scene
	{
	public:
		Scene();
		~Scene();

		// Create a new entity function
		Entity CreateEntity(const std::string& name = std::string());

		// Entity update function
		void OnUpdate(Timestep timestep);

		// Viewport resize function
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}
