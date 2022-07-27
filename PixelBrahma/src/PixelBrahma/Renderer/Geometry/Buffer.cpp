#include "pbpch.h"
#include "Buffer.h"

#include "PixelBrahma/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"	

namespace PixelBrahma
{
	// Vertex buffer create function
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: 
				PB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
				return nullptr;

			case RendererAPI::API::OpenGL: 
				return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		PB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	// Index buffer create function
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			PB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		PB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}