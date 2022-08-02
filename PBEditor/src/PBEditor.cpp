#include <PixelBrahma.h>
#include <PixelBrahma/Application/EntryPoint.h>

#include "EditorLayer.h"

namespace PixelBrahma
{
	// Editor class
	class PBEditor : public Application
	{
	public:
		PBEditor() : Application("PBEditor") 
		{ 
			PushLayer(new EditorLayer()); 
		}

		~PBEditor() {}
	};

	// Create Editor
	Application* CreateApplication()
	{
		return new PBEditor();
	}
}