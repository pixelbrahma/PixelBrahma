#include "pbpch.h"
#include "Scene.h"

#include "Components.h"
#include "PixelBrahma/Renderer/Renderer2D.h"
#include "Entity.h"

#include <glm/glm.hpp>

namespace PixelBrahma
{
	Scene::Scene() {}

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

	// Destroy the entity
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	// Update scene entities
	void Scene::OnUpdate(Timestep timestep)
	{
		// Update scripts
		{
			// Get and set scripting components
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				// If native scripting component instance doesnt exist, create instance and set instance entity
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity(entity, this);

					// Call the create function
					nsc.Instance->OnCreate();
				}

				// Call the update function
				nsc.Instance->OnUpdate(timestep);
			});
		}

		// Render 2D

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			// Group entities with camera component
			auto view = m_Registry.view<TransformComponent, CameraComponent>();

			for (auto entity : view)
			{
				// Get the transform and camera components
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				// Set the main camera
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		// Render from the main camera
		if (mainCamera)
		{
			// Begin rendering
			Renderer2D::BeginScene(*mainCamera, cameraTransform);
			
			// Get entities with sprite renderer component
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for (auto entity : group)
			{
				// Get the transform and sprite renderer components
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				// Draw call
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
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

	// Get the entity with camera component set as primary camera
	Entity Scene::GetPrimaryCameraEntity()
	{
		// Get camera component entities
		auto view = m_Registry.view<CameraComponent>();

		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);

			// Check if the camera is the primary camera and return the entity
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	// On component added template function to call the respective component on added type
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	// Transform component on added 
	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	// Camera component on added 
	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	// Sprite renderer component on added 
	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	// Tag component on added 
	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	// Native script component on added 
	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
}
