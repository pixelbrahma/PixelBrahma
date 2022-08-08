#include "pbpch.h"
#include "Scene.h"

#include "Components.h"
#include "PixelBrahma/Renderer/Renderer2D.h"
#include "Entity.h"
#include "ScriptableEntity.h"

#include <glm/glm.hpp>

// Box2D
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace PixelBrahma
{
	// Convert rigid body component to box2d component type 
	static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2DComponent::BodyType::Static:    return b2_staticBody;
			case Rigidbody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
			case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		PB_CORE_ASSERT(false, "Unknown body type");

		return b2_staticBody;
	}

	Scene::Scene() {}

	Scene::~Scene() {}

	// Create an entity function
	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}

	// Create entity with UUID
	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		// Create an entity in this scene
		Entity entity = { m_Registry.create(), this };
		// Add the uuid component
		entity.AddComponent<IDComponent>(uuid);
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

	// Start of runtime
	void Scene::OnRuntimeStart()
	{
		// Create new physics world
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		// Get rigidbody components
		auto view = m_Registry.view<Rigidbody2DComponent>();

		for (auto e : view)
		{
			// Get entity and component handles

			Entity entity = { e, this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			// Physics body definition
			b2BodyDef bodyDef;
			bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			// Create physics body
			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				// Get collider component
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				// Set collider shape
				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				// Define fixture properties
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;

				// Create fixture
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	// When runtime stops
	void Scene::OnRuntimeStop()
	{
		// Delete the physics simulation world
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	// Update runtime entities
	void Scene::OnUpdateRuntime(Timestep timestep)
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

		// Update physics
		{
			// Physics time step
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			m_PhysicsWorld->Step(timestep, velocityIterations, positionIterations);

			// Retrieve transform from Box2D
			auto view = m_Registry.view<Rigidbody2DComponent>();

			for (auto e : view)
			{
				// Get entity and component handles
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

				// Update body position and rotation based on physics world values
				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				const auto& position = body->GetPosition();
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
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

				// Draw the sprite
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}

			// End rendering
			Renderer2D::EndScene();
		}
	}

	// Update editor
	void Scene::OnUpdateEditor(Timestep timestep, EditorCamera& camera)
	{
		// Begin rendering
		Renderer2D::BeginScene(camera);

		// Get entities with sprite renderer component
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for (auto entity : group)
		{
			// Get the transform and sprite renderer components
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			// Draw the sprite
			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		// End rendering
		Renderer2D::EndScene();
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
		//static_assert(false);
	}

	// UUID component added
	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component) {}

	// Transform component added 
	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {}

	// Camera component added 
	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		// Set camera viewport size
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	// Sprite renderer component added 
	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}

	// Tag component added 
	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {}

	// Native script component added 
	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {}

	// Rigid body component added
	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component) {}

	// Box collider component added
	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component) {}
}
