#include "pbpch.h"
#include "Scene.h"

#include "Components.h"
#include "PixelBrahma/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace PixelBrahma
{
	static void DoMath(const glm::mat4& transform) {}

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity) {}

	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE

		// Create an entity in the registry
		entt::entity entity = m_Registry.create();

		// Add a transform component to the entity
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		// Connect the on construction function to the defined function
		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

		// If the entity has a transform component, get it
		if (m_Registry.has<TransformComponent>(entity))
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);

		// Get all entities with transform components in the registry
		auto view = m_Registry.view<TransformComponent>();

		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		// Group entities with transform and mesh components
		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);

		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}

#endif
	}

	Scene::~Scene() {}

	// Create an entity function
	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	// Update scene entities
	void Scene::OnUpdate(Timestep timestep)
	{
		// group objects with transform and sprite renderer components
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for (auto entity : group)
		{
			// Get the component from the entity
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			// Draw call
			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}