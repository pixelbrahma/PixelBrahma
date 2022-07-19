#include "pbpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace PixelBrahma
{
	// Create vertex array
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None: 
				PB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::OpenGL:
				return new OpenGLVertexArray();
		}

		PB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}