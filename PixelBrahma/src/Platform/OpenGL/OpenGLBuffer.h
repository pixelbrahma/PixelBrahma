#pragma once

#include "PixelBrahma/Renderer/Geometry/Buffer.h"

namespace PixelBrahma
{
	// Vertex buffer implementation for OpenGl
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		// Overrides of base class bind and unbind functions

		virtual void Bind() const override;
		virtual void UnBind() const override;

		// Set vertex buffer data function
		virtual void SetData(const void* data, uint32_t size) override;

		// Overrides of base class buffer layout functions

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	// Index buffer implementation for OpenGl
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		// Overrides of base class bind and unbind functions

		virtual void Bind() const override;
		virtual void UnBind() const override;

		// Get index buffer count function override
		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}