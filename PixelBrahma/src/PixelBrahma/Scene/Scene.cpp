#include "pbpch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"
#include "ScriptableEntity.h"
#include "PixelBrahma/Scripting/ScriptEngine.h"

#include "PixelBrahma/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

// Box2D
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

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

	Scene::~Scene() 
	{
		delete m_PhysicsWorld;
	}

	// Copy a component
	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, 
		const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
		{
			auto view = src.view<Component>();

			for (auto srcEntity : view)
			{
				entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);
				auto& srcComponent = src.get<Component>(srcEntity);
				dst.emplace_or_replace<Component>(dstEntity, srcComponent);
			}
		}(), ...);
	}

	// Copy a component wrapper
	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, 
		const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}


	// If a component already exists, copy values
	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
		{
			if (src.HasComponent<Component>())
				dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}(), ...);
	}

	// If a component already exists, copy values wrapper
	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	// Copy a scene
	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		// New scene reference
		Ref<Scene> newScene = CreateRef<Scene>();

		// Set new scene viewport dimensions
		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		// Copy entity registry
		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		auto idView = srcSceneRegistry.view<IDComponent>();

		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

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

		m_EntityMap[uuid] = entity;

		return entity;
	}

	// Destroy the entity
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
		m_EntityMap.erase(entity.GetUUID());
	}

	// Start of runtime
	void Scene::OnRuntimeStart()
	{
		// Start physics
		OnPhysics2DStart();

		// Scripting
		{
			ScriptEngine::OnRuntimeStart(this);
			// Instantiate all script entities

			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				ScriptEngine::OnCreateEntity(entity);
			}
		}
	}

	// Stop of runtime
	void Scene::OnRuntimeStop()
	{
		// Stop physics
		OnPhysics2DStop();

		// Scripting
		ScriptEngine::OnRuntimeStop();
	}

	// Start physics simulation
	void Scene::OnSimulationStart()
	{
		// Start physics
		OnPhysics2DStart();
	}

	// Stop physics simulation
	void Scene::OnSimulationStop()
	{
		// Stop physics
		OnPhysics2DStop();
	}

	// Update runtime entities
	void Scene::OnUpdateRuntime(Timestep timestep)
	{
		// Update scripts
		{
			// C# Entity OnUpdate
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				ScriptEngine::OnUpdateEntity(entity, timestep);
			}

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
			
			// Draw sprites
			{
				// Get entities with sprite renderer component
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

				for (auto entity : group)
				{
					// Get the transform and sprite renderer components
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					// Draw the sprite
					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				}
			}

			// Draw circles
			{
				// Get entities with circle renderer component
				auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();

				for (auto entity : view)
				{
					// Get the transform and circle renderer components
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

					// Draw the circle
					Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, 
						circle.Thickness, circle.Fade, (int)entity);
				}
			}

			// End rendering
			Renderer2D::EndScene();
		}
	}

	// Simulation - Physics
	void Scene::OnUpdateSimulation(Timestep timestep, EditorCamera& camera)
	{
		// Physics time step
		{
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

		// Render the scene
		RenderScene(camera);
	}

	// Update editor
	void Scene::OnUpdateEditor(Timestep timestep, EditorCamera& camera)
	{
		// Render scene
		RenderScene(camera);
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

	// Duplicate an entity
	void Scene::DuplicateEntity(Entity entity)
	{
		Entity newEntity = CreateEntity(entity.GetName());
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
	}

	// Get entity unique identifier
	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };

		return {};
	}

	// Start physics
	void Scene::OnPhysics2DStart()
	{
		//// Physics ////

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

			// Box collider component
			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				// Get box collider component
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

			// Circle collider component
			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				// Get circle collider component
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				// Set collider shape
				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = transform.Scale.x * cc2d.Radius;

				// Define fixture properties
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;

				// Create fixture
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	// Stop physics
	void Scene::OnPhysics2DStop()
	{
		// Delete the physics world
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	// Render the scene
	void Scene::RenderScene(EditorCamera& camera)
	{
		// Begin rendering
		Renderer2D::BeginScene(camera);

		// Draw sprites
		{
			// Get entities with sprite renderer component
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for (auto entity : group)
			{
				// Get the transform and sprite renderer components
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				// Draw the sprite
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}
		}

		// Draw cirles
		{
			// Get entities with circle renderer component
			auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();

			for (auto entity : view)
			{
				// Get the transform and circle renderer components
				auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

				// Draw the circle
				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color,
					circle.Thickness, circle.Fade, (int)entity);
			}
		}

		// End rendering
		Renderer2D::EndScene();
	}

	// On component added template function to call the respective component on added type
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
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

	// Mono script component added
	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component) {}

	// Sprite renderer component added 
	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {}

	// Circle renderer component added
	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component) {}

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

	// Circle collider component added
	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component) {}
}
