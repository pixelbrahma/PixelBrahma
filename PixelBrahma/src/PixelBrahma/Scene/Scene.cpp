#include "pbpch.h"
#include "Scene.h"

#include "Components.h"
#include "PixelBrahma/Renderer/Renderer2D.h"
#include "Entity.h"

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
	Entity Scene::CreateEntity(const std::string& name)
	{
		// Create an entity in this scene
		Entity entity = { m_Registry.create(), this };
		// Add a transform component to the entity
		entity.AddComponent<TransformComponent>();
		// Add a tag component to the entity
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	// Update scene entities
	void Scene::OnUpdate(Timestep timestep)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			// Group entities with camera component
			auto view = m_Registry.view<TransformComponent, CameraComponent>();

			for (auto entity : view)
			{
				// Get the transform and camera components
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				// Set the main camera
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		// Render from the main camera
		if (mainCamera)
		{
			// Begin rendering
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
			
			// Get entities with sprite renderer component
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for (auto entity : group)
			{
				// Get the transform and sprite renderer components
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				// Draw call
				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			// End rendering
			Renderer2D::EndScene();
		}
	}

	// Viewport resize function
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		// Set viewport dimensions to new values

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize the non - fixed aspect ratio cameras

		// Get entities with camera component
		auto view = m_Registry.view<CameraComponent>();

		for (auto entity : view)
		{
			// Get the cammera component
			auto& cameraComponent = view.get<CameraComponent>(entity);

			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}
	}
}