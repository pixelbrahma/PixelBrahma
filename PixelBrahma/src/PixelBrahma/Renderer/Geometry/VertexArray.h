#pragma once

#include <memory>
#include <vector>

#include "PixelBrahma/Renderer/Geometry/Buffer.h"

namespace PixelBrahma
{
	// Vertex array interface
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		// Vertex array bind and unbind functions

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		// Vertex and index buffer set functions

		virtual	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		// Vertex and index buffer get functions

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		// Vertex array create functions

		static VertexArray* Create();
	};
}