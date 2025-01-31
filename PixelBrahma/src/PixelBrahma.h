#pragma once

// For use by Applications

#include <stdio.h>

// Core
#include "PixelBrahma/Core/Core.h"
#include "PixelBrahma/Core/Assert.h"

// Application
#include "PixelBrahma/Application/Application.h"

// Layer
#include "PixelBrahma/Layers/Layer.h"

// Log
#include "PixelBrahma/Log/Log.h"

// Timestep
#include "PixelBrahma/Core/Timestep.h"

// Input
#include "PixelBrahma/Input/Input.h"
#include "PixelBrahma/Input/KeyCodes.h"
#include "PixelBrahma/Input/MouseCodes.h"

// ImGui
#include "PixelBrahma/ImGui/ImGuiLayer.h"

// ECS
#include "PixelBrahma/Scene/Scene.h"
#include "PixelBrahma/Scene/Entity.h"
#include "PixelBrahma/Scene/Components.h"
#include "PixelBrahma/Scene/ScriptableEntity.h"

// Renderer
#include "PixelBrahma/Renderer/Renderer.h"
#include "PixelBrahma/Renderer/Renderer2D.h"
#include "PixelBrahma/Renderer/RenderCommand.h"

// Framebuffer
#include "PixelBrahma/Renderer/Framebuffer.h"

// Render objects
#include "PixelBrahma/Renderer/Geometry/VertexArray.h"
#include "PixelBrahma/Renderer/Geometry/Buffer.h"
#include "PixelBrahma/Renderer/Shading/Shader.h"
#include "PixelBrahma/Renderer/Shading/Texture.h"

// Camera
#include "PixelBrahma/Renderer/Camera/OrthographicCamera.h"
#include "PixelBrahma/Renderer/Camera/OrthographicCameraController.h"
