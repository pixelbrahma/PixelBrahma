#include <PixelBrahma.h>
#include <PixelBrahma/Application/EntryPoint.h>

#include "EditorLayer.h"

namespace PixelBrahma
{
	// Editor class
	class PBEditor : public Application
	{
	public:
		PBEditor(ApplicationCommandLineArgs args) : Application("PBEditor", args)
		{ 
			PushLayer(new EditorLayer()); 
		}

		~PBEditor() {}
	};

	// Create Editor
	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new PBEditor(args);
	}
}
