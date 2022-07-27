#pragma once

#include "PixelBrahma/Renderer/Geometry/VertexArray.h"

namespace PixelBrahma
{
	// OpenGL platform specific vertex array
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		// Overriding vertex array bind and unbind base class functions

		virtual void Bind() const override;
		virtual void UnBind() const override;

		// Vertex and index buffer setter functions

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		// Vertex and index buffer getter functions

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override
		{ return m_VertexBuffers; }
		
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}