#include "pbpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace PixelBrahma
{
	// Set static renderer API to OpenGL renderer API
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}