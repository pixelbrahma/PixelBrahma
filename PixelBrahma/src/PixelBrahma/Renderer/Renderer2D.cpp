#include "pbpch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"

#include "Geometry/VertexArray.h"

#include "Shading/Shader.h"
#include "Shading/UniformBuffer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

		// Editor-only
		int EntityID;
	};

	// Circle vertex data structure
	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Editor-only
		int EntityID;
	};

	// Line vertex data structure
	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor-only
		int EntityID;
	};

	// Structure with renderer object data
	struct Renderer2DData
	{
		// Batch Rendering batch size

		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		// Quad objects

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture;

		// Circle objects

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		// Line objects

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;
		
		// Vertex buffer batching variables

		// Quads

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// Circles

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		// Lines

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 2.0f;

		// Textures batching variables

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;		// Because slot 0 is the dummy texture

		glm::vec4 QuadVertexPositions[4];

		// Stats
		Renderer2D::Statistics Stats;

		// Camera data structure
		struct CameraData
		{
			glm::mat4 ViewProjection;
		};

		// Camera buffers
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData s_Data;

	// Initialize function
	void Renderer2D::Init()
	{
		// Profiling
		PB_PROFILE_FUNCTION();
		
		//// Quads ////

		// Set storage vertex array
		s_Data.QuadVertexArray = VertexArray::Create();

		// Create vertex buffer, set layout and add it to the vertex array
		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" },
			{ ShaderDataType::Int,    "a_EntityID"     }
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

		//// Circles ////

		// Set storage vertex array
		s_Data.CircleVertexArray = VertexArray::Create();

		// Create vertex buffer, set layout and add it to the vertex array
		s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));

		s_Data.CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float,  "a_Thickness"     },
			{ ShaderDataType::Float,  "a_Fade"          },
			{ ShaderDataType::Int,    "a_EntityID"      }
			});

		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(quadIB); // Use quad IB

		// Array of vertex buffer vertices
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

		//// Lines ////

		// Set storage vertex array
		s_Data.LineVertexArray = VertexArray::Create();

		// Create vertex buffer, set layout and add it to the vertex array
		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));

		s_Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Int,    "a_EntityID" }
			});

		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);

		// Array of vertex buffer vertices
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

		// Create default texture
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		// Texture samplers for each texture
		int32_t samplers[s_Data.MaxTextureSlots];

		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		// Create and bind shaders from file path
		s_Data.QuadShader = Shader::Create("Assets/Shaders/Renderer2D_Quad.glsl");
		s_Data.CircleShader = Shader::Create("Assets/Shaders/Renderer2D_Circle.glsl");
		s_Data.LineShader = Shader::Create("Assets/Shaders/Renderer2D_Line.glsl");

		// Set dummy texture to the first texture slot
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		// Set quad vertex positions
		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		// Create uniform buffer for camera data
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
	}

	// Shutdown function for cleanup
	void Renderer2D::Shutdown() 
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
	}

	// Begin scene functions

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Get camera view projection matrix and push data to the camera uniform buffer
		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		// Start batching
		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Get camera view projection matrix and push data to the camera uniform buffer
		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		// Start batching
		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		// Get camera view projection matrix and push data to the camera uniform buffer
		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		// Start batching
		StartBatch();
	}

	// End Scene cleanup function
	void Renderer2D::EndScene() 
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		Flush();
	}

	// Start batch rendering
	void Renderer2D::StartBatch()
	{
		// Quads
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		// Circles
		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		// Lines
		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	// Flush buffers
	void Renderer2D::Flush()
	{
		// If its a quad
		if (s_Data.QuadIndexCount)
		{
			// Set the pointer to the end of the batch and flush the buffer 

			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			// Bind all the textures in the texture slots
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

			// Bind the shader
			s_Data.QuadShader->Bind();

			// Draw call for batched quads
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		// If its a circle
		if (s_Data.CircleIndexCount)
		{
			// Set the pointer to the end of the batch and flush the buffer 

			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - 
				(uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

			// Bind the texture
			s_Data.CircleShader->Bind();

			// Draw call for batched circles
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		// If its a line
		if (s_Data.LineVertexCount)
		{
			// Set the pointer to the end of the batch and flush the buffer 

			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

			// Bind the texture
			s_Data.LineShader->Bind();
			// Set line wicth
			RenderCommand::SetLineWidth(s_Data.LineWidth);

			// Draw call for batched circles
			RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
			s_Data.Stats.DrawCalls++;
		}
	}

	// Flush and end rendering
	void Renderer2D::NextBatch()
	{
		// Flush and start the next batch

		Flush();
		StartBatch();
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

		// Transformation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
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

		// Transformation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f;		// Dummy texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		// Set quad vertex buffer pointer attributes for each of the quad vertices

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position        = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color           = color;
			s_Data.QuadVertexBufferPtr->TexCoord        = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex        = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor    = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID        = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor,
		const glm::vec4& tintColor, int entityID)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;

		// Find and set index of the texture
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		// Set quad vertex buffer pointer attributes for each of the quad vertices

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position        = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color           = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord        = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex        = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor    = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID        = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;
		s_Data.Stats.QuadCount++;
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

		// Set transformation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.QuadShader->SetMat4("u_Transform", transform);

		DrawQuad(transform, color);
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

		// Set transformation matrix
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	// Draw Circle
	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, 
		float thickness /*= 1.0f*/, float fade /*= 0.005f*/, int entityID /*= -1*/)
	{
		// Profiling
		PB_PROFILE_FUNCTION();

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition    = transform * s_Data.QuadVertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition    = s_Data.QuadVertexPositions[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color            = color;
			s_Data.CircleVertexBufferPtr->Thickness        = thickness;
			s_Data.CircleVertexBufferPtr->Fade             = fade;
			s_Data.CircleVertexBufferPtr->EntityID         = entityID;
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	// Draw line
	void Renderer2D::DrawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		s_Data.LineVertexBufferPtr->Position               = p0;
		s_Data.LineVertexBufferPtr->Color                  = color;
		s_Data.LineVertexBufferPtr->EntityID               = entityID;
		s_Data.LineVertexBufferPtr++;		               
											               
		s_Data.LineVertexBufferPtr->Position               = p1;
		s_Data.LineVertexBufferPtr->Color                  = color;
		s_Data.LineVertexBufferPtr->EntityID               = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
	}

	// Draw rect functions

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		// Points of the rectangle 

		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		// Draw the lines
		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		// Points of the rectangle 

		glm::vec3 lineVertices[4];

		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * s_Data.QuadVertexPositions[i];

		// Draw the lines
		DrawLine(lineVertices[0], lineVertices[1], color, entityID);
		DrawLine(lineVertices[1], lineVertices[2], color, entityID);
		DrawLine(lineVertices[2], lineVertices[3], color, entityID);
		DrawLine(lineVertices[3], lineVertices[0], color, entityID);
	}

	// Draw sprite function for drawing a sprite
	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.Texture)
			DrawQuad(transform, src.Texture, src.TilingFactor, src.Color, entityID);
		else
			DrawQuad(transform, src.Color, entityID);
	}

	// Get the line width
	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	// Set line width
	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.LineWidth = width;
	}

	// Reset stats
	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	// Get stats
	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
}
