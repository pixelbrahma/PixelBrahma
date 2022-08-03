#pragma once

#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace PixelBrahma
{
	// The tag component
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	// The transform component
	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : Transform(transform) {}

		// Operator overloading to implicitly cast the structure name to the transform field within the structure
		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	// Sprite renderer component
	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	// Camera component
	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; 
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	// Native scripting component
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		// Instance create and destroy functions

		std::function<void()> InstantiateFunction;
		std::function<void()> DestroyInstanceFunction;

		// Instance life cycle functions

		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity*)> OnDestroyFunction;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;

		// Bind template function
		template<typename T>
		void Bind()
		{
			// Create and destroy instance lamdas

			InstantiateFunction = [&]() { Instance = new T(); };
			DestroyInstanceFunction = [&]() { delete (T*)Instance; Instance = nullptr; };

			// Instance life cycle lambdas

			OnCreateFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
			OnDestroyFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnDestroy(); };
			OnUpdateFunction = [](ScriptableEntity* instance, Timestep ts) { ((T*)instance)->OnUpdate(ts); };
		}
	};
}