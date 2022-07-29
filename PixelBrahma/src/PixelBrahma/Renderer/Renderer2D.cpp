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
		float TexIndex;
		float TilingFactor;
	};

	// Structure with renderer object data
	struct Renderer2DData
	{
		// Batch Rendering batch size

		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
		
		// Vertex buffer batching variables

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// Textures batching variables

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;		// Because slot 0 is the dummy texture
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
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" }
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

		// Texture samplers for each texture
		int32_t samplers[s_Data.MaxTextureSlots];

		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		// Create and bind shader from file path
		s_Data.TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		
		// Bind and set uniform slot of the texture shader
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
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

		s_Data.TextureSlotIndex = 1;
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
		// Bind all the textures in the texture slots
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

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

		const float texIndex = 0.0f;		// Dummy texture
		const float tilingFactor = 1.0f;

		// Set quad vertex buffer pointer attributes for each of the quad vertices

		s_Data.QuadVertexBufferPtr->Position        = position;
		s_Data.QuadVertexBufferPtr->Color           = color;
		s_Data.QuadVertexBufferPtr->TexCoord        = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex        = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor    = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position        = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color           = color;
		s_Data.QuadVertexBufferPtr->TexCoord        = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex        = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor    = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position        = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color           = color;
		s_Data.QuadVertexBufferPtr->TexCoord        = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex        = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor    = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position        = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color           = color;
		s_Data.QuadVertexBufferPtr->TexCoord        = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex        = texIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor    = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

#if OLD_PATH
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
#endif
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

		constexpr glm::vec4 color = { 1.0f ,1.0f, 1.0f, 1.0f };
		float textureIndex = 0.0f;

		// Find and set index of the texture
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		// Set quad vertex buffer pointer attributes for each of the quad vertices

		s_Data.QuadVertexBufferPtr->Position       = position;
		s_Data.QuadVertexBufferPtr->Color          = color;
		s_Data.QuadVertexBufferPtr->TexCoord       = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex       = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor   = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position       = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color          = color;
		s_Data.QuadVertexBufferPtr->TexCoord       = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex       = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor   = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position       = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color          = color;
		s_Data.QuadVertexBufferPtr->TexCoord       = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex       = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor   = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position       = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color          = color;
		s_Data.QuadVertexBufferPtr->TexCoord       = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex       = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor   = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

#if OLD_PATH
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
#endif
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