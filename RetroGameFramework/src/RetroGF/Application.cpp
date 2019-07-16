#include "RGFpch.h"
#include "Application.h"


#include "RetroGF/Platform/Windows/WindowsWindow.h"

#include <RetroGF/Rendering/API/Buffer.h>
#include <RetroGF/Rendering/API/VertexArray.h>
#include <RetroGF/Rendering/API/Shader.h>
#include <RetroGF/Rendering/API/Texture.h>
#include <RetroGF/Rendering/API/FrameBuffer.h>


#include <GLM/glm/gtc/matrix_transform.hpp>
#include <RetroGF/Utility/Random.h>

#include <RetroGF/Rendering/Renderer2D.h>
#include <RetroGF/Rendering/Sprite.h>

#include "RetroGF/Platform/OpenGL/GLCommon.h"

#include "RetroGF/Input.h"
#include "RetroGF/KeyCodes.h"


namespace RGF {



#define Batchrendering 0

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		s_Instance = this;
		m_Window = std::unique_ptr<WindowImpl>(WindowImpl::Create({960,540}));

		m_ShaderManager = std::make_unique<ShaderManager>();
#if Batchrendering 
		m_Renderer = std::unique_ptr<RGF::Renderer2D>(RGF::Renderer2D::Create(RenderingType::Batch));
#else
		m_Renderer = std::unique_ptr<RGF::Renderer2D>(RGF::Renderer2D::Create(RenderingType::Default));
#endif

		// Bind the "OnEvent" to the function pointer in "WindowImpl.h"
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));


#ifndef RGF_DISTRIBUTE
		m_EngineEditorLayer = new ImguiEngineEditor;
#endif


		m_Camera = std::make_unique<Camera>();
		RGF_CORE_MSG("Initialising File IO!\n");
		m_FileIO = std::make_unique<FileIO>();
		RGF_CORE_MSG("Creating the renderer!\n");
		m_Renderer->Init();

		RGF_CORE_TRACE("RGF application created!\n");
		RGF_CORE_TRACE("Time took to init application: %fms\n", m_AppTimer.GetElapsedMillis());


		m_ShaderManager->Add(ShaderGenerator::GetInstance()->DefaultShader(), "Default Shader");
		m_ShaderManager->Add(ShaderGenerator::GetInstance()->TexturedShader(), "Textured Shader");

	}
	Application::~Application() {
	}


	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);

	}
	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}



	// Is bound to the function pointer in "m_Window". This function will be called when a event happens.
	void Application::OnEvent(Event& e) {

		EventDispatcher dispatcher(e);

		// Checks if the event was a "WindowCloseEvent". If it was the event, call the "OnWindowClose" function.
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

		for (Layer* layer : m_LayerStack.GetLayerStack()) {
			layer->OnEvent(e);
			if (e.Handled)
				break;
		}

	}


	void Application::Run() {
		float FPS = 60.0f;
		float Time = 0.0f;
		float UpdateTimer = 0.0f;
		float UpdateTick = 1.0f / FPS;
		unsigned int Frames = 0;
		unsigned int Updates = 0;





		while (m_IsRunning) {
#ifndef RGF_DISTRIBUTE
			m_EngineEditorLayer->GetEditor().GetGameViewport().ViewportFBO->Bind();
#endif
			m_Renderer->Clear();


			if (m_AppTimer.GetElapsedMillis() - UpdateTimer > UpdateTick) {
				Updates++;
				UpdateTimer += UpdateTick;
			}
			Frames++;

			for (Layer* layer : m_LayerStack.GetLayerStack()) {
				layer->OnUpdate(0.0f);
			}

			
#ifndef RGF_DISTRIBUTE
			m_EngineEditorLayer->GetEditor().GetGameViewport().ViewportFBO->Unbind();
			m_EngineEditorLayer->GetEditor().GetGameViewport().ViewportFBO->Clear();

			m_EngineEditorLayer->Start();
			for (Layer* layer : m_LayerStack.GetLayerStack()) {
				layer->OnImguiRender();
			}
			m_EngineEditorLayer->End();
#endif


			m_Window->OnUpdate();

			if (m_AppTimer.GetElapsedMillis() - Time > 1.0f) {
				Time += 1.0f;
				RGF_CORE_MSG("%d: FPS\t%d: UPS\n", Frames, Updates);
#ifdef RGF_DISTRIBUTE
				printf("%d: FPS\t%d: UPS\n", Frames, Updates);
#endif
				Frames = 0;
				Updates = 0;
			}
		}

	}


	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_IsRunning = false;
		return true;
	}



}