#pragma once

namespace PixelBrahma 
{
	// Script engine to handle c# scripting support
	class ScriptEngine
	{
	public:
		// Engine initilialize and shutdown functions
		static void Init();
		static void Shutdown();

	private:
		// Mono initialize and shutdown functions
		static void InitMono();
		static void ShutdownMono();
	};

}
