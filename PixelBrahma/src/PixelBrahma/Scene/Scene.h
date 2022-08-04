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

		// Entity create and destroy functions

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		// Entity update function
		void OnUpdate(Timestep timestep);

		// Viewport resize function
		void OnViewportResize(uint32_t width, uint32_t height);

	private:

		// On component added template function
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}
