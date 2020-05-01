#pragma once

// Includes all header files from RetroGameFramework.


// Core.
#include "RetroGF/Core/Application.h"
#include "RetroGF/Core/Input.h"
#include "RetroGF/Core/KeyCodes.h"
#include "RetroGF/Core/MouseButtonCodes.h"
#include "RetroGF/Core/LayerStack.h"
#include "RetroGF/Core/Layer.h"
#include "RetroGF/Core/OrthographicCameraController.h"


// Utility.
#include "RetroGF/Utility/File.h"
#include "RetroGF/Utility/Random.h"
#include "RetroGF/Utility/Timer.h"


// Events.
#include "RetroGF/Events/Event.h"
#include "RetroGF/Events/KeyEvents.h"
#include "RetroGF/Events/MouseEvents.h"
#include "RetroGF/Events/WindowEvents.h"



// ---- ENTRY POINT ---- \\.
#include "RetroGF/Core/EntryPoint.h"
// ---- ---------- ---- \\.


// ---- PCH ---- \\.
#include "RGFpch.h"
// ---- ---------- ---- \\.


// Rendering.
#include "RetroGF/Rendering/API/Buffer.h"
#include "RetroGF/Rendering/API/VertexArray.h"

#include "RetroGF/Rendering/API/Shader/ShaderGenerator.h"
#include "RetroGF/Rendering/API/Shader/ShaderManager.h"
#include "RetroGF/Rendering/API/Shader/ShaderUniforms.h"
#include "RetroGF/Rendering/API/Texture.h"

#include "RetroGF/Rendering/RenderingContext.h"
#include "RetroGF/Rendering/Renderable.h"
#include "RetroGF/Rendering/OrthographicCamera.h"
#include "RetroGF/Rendering/ParticleSystem.h"

#include "RetroGF/Rendering/Renderer2D.h"
#include "RetroGF/Rendering/RenderCommand.h"

// Debug.
#include "RetroGF/Debug/Instrumentor.h"