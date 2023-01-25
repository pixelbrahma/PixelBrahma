#pragma once

#include <filesystem>
#include <string>

extern "C" 
{
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
}

namespace PixelBrahma 
{
	// Script engine to handle c# scripting support
	class ScriptEngine
	{
	public:
		// Engine initilialize and shutdown functions
		static void Init();
		static void Shutdown();

		// Load C# assembly from file function
		static void LoadAssembly(const std::filesystem::path& filepath);

	private:
		// Mono initialize and shutdown functions
		static void InitMono();
		static void ShutdownMono();

		// Instantiate Moco class function
		static MonoObject* InstantiateClass(MonoClass* monoClass);

		friend class ScriptClass;
	};

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
}
