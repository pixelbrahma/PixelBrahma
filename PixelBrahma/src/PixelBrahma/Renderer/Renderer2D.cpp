#include "pbpch.h"
#include "Renderer2D.h"

#include "Geometry/VertexArray.h"
#include "Shading/Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace PixelBrahma
{
	// Quad vertex data structure
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	// Structure with renderer object data
	struct Renderer2DData
	{
		// Batch Rendering batch size

		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
	};

	static Renderer2DData s_Data;

	// Initialize function
	void Renderer2D::Init()
	{
		// Profiling
		PB_PROFILE_FUNCTION();
		
		// Set storage vertex array
		s_Data.QuadVertexArray = VertexArray::Create();
/*
		// Vertices
		float squareVertices[5 * 4] = {
			// Positions            // Texture coordinates
			-0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,     1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,     1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,     0.0f, 1.0f
		};
*/
		// Create vertex buffer, set layout and add it to the vertex array
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		// Array of vertex buffer vertices
		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		// Array of indices
		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;

		// Set index offsets
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		
		// Index buffer
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// Create default texture
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// Create and bind shader from file path
		s_Data.TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		
		// Bind and set uniform slot of the texture shader
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetInt("u_Texture", 0);
	}

	// Shutdown function for cleanup
	void Renderer2D::Shutdown() 
	{
		// Profiling
		PB_PROFILE_FUNCTION();
	}

	// Begin scene function
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	// End Scene cleanup function
	void Renderer2D::EndScene() 
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Set the pointer to the end of the batch and flush the buffer 

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		// Draw call for batched quads
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	// Draw Quad functions

	// Draw quad

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Set quad vertex buffer pointer attributes for each of the quad vertices

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
/*
		// Bind shaders and upload uniforms
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data->WhiteTexture->Bind();

		// Set transformation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		// Bind vertex array
		s_Data->QuadVertexArray->Bind();

		// Draw call
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
		float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, 
		float tilingFactor, const glm::vec4& tintColor)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Bind the texture and upload uniforms
		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		// Set transformation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		// Bind vertex array
		s_Data.QuadVertexArray->Bind();

		// Draw call
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}

	// Draw rotated quad

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, 
		const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, 
		const glm::vec4& color)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Bind the texture and upload uniforms
		s_Data.TextureShader->SetFloat4("u_Color", color);
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.WhiteTexture->Bind();

		// Set transformation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		// Bind vertex array
		s_Data.QuadVertexArray->Bind();

		// Draw call
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, 
		const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, 
		const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Bind the texture and upload uniforms
		s_Data.TextureShader->SetFloat4("u_Color", tintColor);
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();

		// Set transformation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.TextureShader->SetMat4("u_Transform", transform);

		// Bind vertex array
		s_Data.QuadVertexArray->Bind();

		// Draw call
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}

}