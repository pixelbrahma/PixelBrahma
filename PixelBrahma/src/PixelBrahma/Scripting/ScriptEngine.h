#pragma once

#include "PixelBrahma/Scene/Scene.h"
#include "PixelBrahma/Scene/Entity.h"

#include <filesystem>
#include <string>

extern "C" 
{
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
}

namespace PixelBrahma 
{
	// Script class to handle script lifetime
	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className);

		// Mono handler functions
		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		MonoClass* m_MonoClass = nullptr;
	};

	// Script Instance class
	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		// Method invoke functions
		void InvokeOnCreate();
		void InvokeOnUpdate(float timestep);

	private:
		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
	};

	// Script engine to handle c# scripting support
	class ScriptEngine
	{
	public:
		// Engine initilialize and shutdown functions
		static void Init();
		static void Shutdown();

		// Load C# assembly from file function
		static void LoadAssembly(const std::filesystem::path& filepath);

		// Play mode start and stop callback functions
		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		// Entity management functions
		static bool EntityClassExists(const std::string& fullClassName);
		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, Timestep timestep);

		// Scene management functions
		static Scene* GetSceneContext();
		static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();

		// Get mono core assembly image
		static MonoImage* GetCoreAssemblyImage();

	private:
		// Mono initialize and shutdown functions
		static void InitMono();
		static void ShutdownMono();

		// Instantiate Mono class function
		static MonoObject* InstantiateClass(MonoClass* monoClass);

		// Load mono assembly classes
		static void LoadAssemblyClasses(MonoAssembly* assembly);

		friend class ScriptClass;
		friend class ScriptGlue;
	};
}
