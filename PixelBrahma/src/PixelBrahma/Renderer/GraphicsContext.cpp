#include "pbpch.h"
#include "GraphicsContext.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace PixelBrahma
{
	// Create graphics context function
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: 
				PB_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		PB_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}
}
