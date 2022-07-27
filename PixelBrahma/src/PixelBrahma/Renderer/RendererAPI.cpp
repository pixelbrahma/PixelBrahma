#include "pbpch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace PixelBrahma
{
	// Set render API to OpenGL
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	// Create renderer API
	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:    
				PB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
				return nullptr;

			case RendererAPI::API::OpenGL:  
				return CreateScope<OpenGLRendererAPI>();
		}

		PB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}