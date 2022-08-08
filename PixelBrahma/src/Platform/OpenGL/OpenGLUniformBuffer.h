#pragma once

#include "PixelBrahma/Renderer/Shading/UniformBuffer.h"

namespace PixelBrahma
{
	// OpenGL platform uniform buffer class
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer();

		// Set uniform data function override
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		uint32_t m_RendererID = 0;
	};
}
