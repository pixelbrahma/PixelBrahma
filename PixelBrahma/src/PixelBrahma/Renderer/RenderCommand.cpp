#include "pbpch.h"
#include "RenderCommand.h"

namespace PixelBrahma
{
	// Set static renderer API to OpenGL renderer API
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}