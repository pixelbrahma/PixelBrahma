#include "pbpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace PixelBrahma
{
	//// Vertex Buffer ////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Create, bind and set vertex buffer data
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Delete vertex buffer
		glDeleteBuffers(1, &m_RendererID);
	}

	// Vertex buffer bind function
	void OpenGLVertexBuffer::Bind() const
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	// Vertex buffer unbind function
	void OpenGLVertexBuffer::UnBind() const
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//// Index Buffer ////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Create, bind and set index buffer data
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Delete Index buffer
		glDeleteBuffers(1, &m_RendererID);
	}

	// Index buffer bind function
	void OpenGLIndexBuffer::Bind() const
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	// Index buffer unbind function
	void OpenGLIndexBuffer::UnBind() const
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}