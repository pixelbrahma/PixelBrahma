#pragma once

#include <entt.hpp>

#include "PixelBrahma/Core/UUID.h"
#include "PixelBrahma/Core/Timestep.h"

#include "PixelBrahma/Renderer/Camera/EditorCamera.h"

class b2World;

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

		// Copy of the editing scene
		static Ref<Scene> Copy(Ref<Scene> other);

		// Entity create and destroy functions

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		// Scene runtime start and stop functions

		void OnRuntimeStart();
		void OnRuntimeStop();

		// Physics simulation start and stop functions

		void OnSimulationStart();
		void OnSimulationStop();

		// Scene update functions

		void OnUpdateRuntime(Timestep timestep);
		void OnUpdateSimulation(Timestep timestep, EditorCamera& camera);
		void OnUpdateEditor(Timestep timestep, EditorCamera& camera);

		// Viewport resize function
		void OnViewportResize(uint32_t width, uint32_t height);

		// Duplicate an entity function
		void DuplicateEntity(Entity entity);

		// Get entity by unique identifier
		Entity GetEntityByUUID(UUID uuid);

		// Get the primary camera in the scene function
		Entity GetPrimaryCameraEntity();

		// Get all entities in the scene with a component template function
		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

	private:

		// On component added template function
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		// Physics start and stop functions

		void OnPhysics2DStart();
		void OnPhysics2DStop();

		// Scene render function
		void RenderScene(EditorCamera& camera);

	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}
