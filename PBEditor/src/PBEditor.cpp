#include "EditorLayer.h"

#include <PixelBrahma.h>
#include <PixelBrahma/Application/EntryPoint.h>

namespace PixelBrahma
{
	// Editor class
	class PBEditor : public Application
	{
	public:
		PBEditor(const ApplicationSpecification& spec) : Application(spec)
		{ 
			PushLayer(new EditorLayer()); 
		}
	};

	// Create Editor
	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.Name = "PBEditor";
		spec.CommandLineArgs = args;

		return new PBEditor(spec);
	}
}
