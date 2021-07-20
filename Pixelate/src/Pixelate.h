#pragma once

// Includes all header files from Pixelate.


// Core.
#include "Pixelate/Core/Application.h"
#include "Pixelate/Core/Input.h"
#include "Pixelate/Core/KeyCodes.h"
#include "Pixelate/Core/MouseButtonCodes.h"
#include "Pixelate/Core/LayerStack.h"
#include "Pixelate/Core/Layer.h"
#include "Pixelate/Core/Random.h"
#include "Pixelate/Core/Timer.h"




// Events.
#include "Pixelate/Events/Event.h"
#include "Pixelate/Events/KeyEvents.h"
#include "Pixelate/Events/MouseEvents.h"
#include "Pixelate/Events/WindowEvents.h"



// ---- ENTRY POINT ---- \\.
#include "Pixelate/Core/EntryPoint.h"
// ---- ---------- ---- \\.


// ---- PCH ---- \\.
#include "PXpch.h"
// ---- ---------- ---- \\.


// Rendering.
#include "Pixelate/Rendering/API/Buffer.h"
#include "Pixelate/Rendering/API/VertexArray.h"

#include "Pixelate/Rendering/API/Shader/ShaderGenerator.h"
#include "Pixelate/Rendering/API/Shader/ShaderManager.h"
#include "Pixelate/Rendering/API/Shader/ShaderUniforms.h"
#include "Pixelate/Rendering/API/Texture.h"
#include "Pixelate/Rendering/API/Framebuffer.h"

#include "Pixelate/Rendering/RendererAPI.h"
#include "Pixelate/Rendering/OrthographicCamera.h"
#include "Pixelate/Rendering/ParticleSystem.h"

#include "Pixelate/Rendering/Renderer2D.h"
#include "Pixelate/Rendering/RenderCommand.h"


// Audio.

#include "Pixelate/Audio/AudioBuffer.h"
#include "Pixelate/Audio/AudioListener.h"
#include "Pixelate/Audio/AudioSource.h"
#include "Pixelate/Audio/Audio.h"

// Physics.

#include "Pixelate/Physics/RigidBody.h"
#include "Pixelate/Physics/Physics.h"

// Debug.
#include "Pixelate/Debug/Instrumentor.h"

// Scene.
#include "Pixelate/Scene/Components.h"
#include "Pixelate/Scene/Entity.h"
#include "Pixelate/Scene/Scene.h"