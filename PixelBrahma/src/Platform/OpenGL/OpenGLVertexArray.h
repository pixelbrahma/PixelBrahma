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

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		// Vertex and index buffer getter functions

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override
		{ return m_VertexBuffers; }
		
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}