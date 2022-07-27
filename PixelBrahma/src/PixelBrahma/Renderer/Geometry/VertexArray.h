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

		virtual	void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual	void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		// Vertex and index buffer get functions

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		// Vertex array create functions

		static Ref<VertexArray> Create();
	};
}