#include "pbpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace PixelBrahma
{
	// Create texture 2D function
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: 
				PB_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return std::make_shared<OpenGLTexture2D>(path);
		}

		PB_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}