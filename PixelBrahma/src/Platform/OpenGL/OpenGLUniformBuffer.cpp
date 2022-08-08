#include "pbpch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace PixelBrahma
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding)
	{
		// Create and bind uniform buffer
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		// Delete uniform buffer
		glDeleteBuffers(1, &m_RendererID);
	}

	// Set uniform buffer data
	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_RendererID, offset, size, data);
	}
}
