#pragma once

#include "PixelBrahma/Renderer/Buffer.h"

namespace PixelBrahma
{
	// Vertex buffer implementation for OpenGl
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		// Overrides of base class bind and unbind functions

		virtual void Bind() const;
		virtual void UnBind() const;

	private:
		uint32_t m_RendererID;
	};

	// Index buffer implementation for OpenGl
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		// Overrides of base class bind and unbind functions

		virtual void Bind() const;
		virtual void UnBind() const;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}