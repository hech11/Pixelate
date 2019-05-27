#include "RGFpch.h"
#include "Application.h"


#include "RetroGF/Platform/Windows/WindowsWindow.h"

#include <RetroGF/Rendering/API/VertexBuffer.h>
#include <RetroGF/Rendering/API/VertexArray.h>
#include <RetroGF/Rendering/API/IndexBuffer.h>
#include <RetroGF/Rendering/API/VertexBufferLayout.h>
#include <RetroGF/Rendering/API/Shader.h>
#include <RetroGF/Rendering/API/Texture.h>
#include <RetroGF/Rendering/API/FrameBuffer.h>


#include <GLM/glm/gtc/matrix_transform.hpp>
#include <RetroGF/FileSystem.h>

#include <RetroGF/Rendering/BatchedSprite.h>
#include <RetroGF/Rendering/Renderer2D.h>
#include <RetroGF/Rendering/Sprite.h>

#include "RetroGF/Platform/OpenGL/GLCommon.h"

namespace RGF {

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		s_Instance = this;
		m_Window = std::unique_ptr<WindowImpl>(WindowImpl::Create({960,540}));
		m_Renderer = std::unique_ptr<RGF::Renderer2D>(RGF::Renderer2D::Create(RenderingType::Default));

		// Bind the "OnEvent" to the function pointer in "WindowImpl.h"
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));


#ifndef RGF_DISTRIBUTE
		// These are deleted by the layerstack.
		m_ImguiLayer = new ImguiLayer();
		m_EngineEditorLayer = new ImguiEngineEditor();
		PushOverlay(m_ImguiLayer);
		PushOverlay(m_EngineEditorLayer);
		PushOverlay(m_EngineEditorLayer->GameView);
		PushOverlay(m_EngineEditorLayer->RenderingProps);
		PushOverlay(m_EngineEditorLayer->EngineColEditor);
#endif
		RGF_CORE_TRACE("RGF application created!\n");
		RGF_CORE_TRACE("Time took to init application: %fms\n", m_AppTimer.GetElapsedMillis());


		m_Renderer->Init();
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



		m_Renderer->Init();

		Shader* shader = Shader::Create();
		shader->LoadFromSrc("res/shader/test.shader");
		shader->Bind();
		Sprite sprite({ 0.0f,0.0f,0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, shader);

		while (m_IsRunning) {

			m_EngineEditorLayer->GameView->ViewportFBO->Bind();
			m_Renderer->Clear();

			for (Layer* layer : m_LayerStack.GetLayerStack()) {
				layer->OnUpdate();
			}


			if (m_AppTimer.GetElapsedMillis() - UpdateTimer > UpdateTick) {
				Updates++;
				UpdateTimer += UpdateTick;
			}
			Frames++;

			m_Renderer->Start();
			m_Renderer->Submit(&sprite);
			m_Renderer->End();
			m_Renderer->Render();


			
#ifndef RGF_DISTRIBUTE
			m_EngineEditorLayer->GameView->ViewportFBO->Unbind();
			m_EngineEditorLayer->GameView->ViewportFBO->Clear();

			m_ImguiLayer->Start();
			for (Layer* layer : m_LayerStack.GetLayerStack()) {
				layer->OnImguiRender();
			}
			m_ImguiLayer->End();
#endif


			m_Window->OnUpdate();

			if (m_AppTimer.GetElapsedMillis() - Time > 1.0f) {
				Time += 1.0f;
				//RGF_CORE_MSG("%d: FPS\t%d: UPS\n", Frames, Updates);
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