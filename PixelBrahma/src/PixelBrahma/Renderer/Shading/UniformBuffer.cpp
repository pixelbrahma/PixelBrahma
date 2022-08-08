#include "pbpch.h"
#include "UniformBuffer.h"

#include "PixelBrahma/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace PixelBrahma
{
	// Create Uniform buffer 
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    
			{
				PB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}

			case RendererAPI::API::OpenGL:  
				return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		PB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
