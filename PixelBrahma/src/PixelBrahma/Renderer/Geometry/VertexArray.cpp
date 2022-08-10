#include "pbpch.h"
#include "VertexArray.h"

#include "PixelBrahma/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace PixelBrahma
{
	// Create vertex array
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: 
				PB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexArray>();
		}

		PB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
