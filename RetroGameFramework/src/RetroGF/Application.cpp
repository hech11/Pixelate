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
#include <RetroGF/Random.h>

#include <RetroGF/Rendering/Renderer2D.h>
#include <RetroGF/Rendering/Sprite.h>

#include "RetroGF/Platform/OpenGL/GLCommon.h"
#include "RetroGF/Rendering/ShaderGenerator.h"

#include "RetroGF/Input.h"
#include "RetroGF/KeyCodes.h"


namespace RGF {



#define Batchrendering 0

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		s_Instance = this;
		m_Window = std::unique_ptr<WindowImpl>(WindowImpl::Create({960,540}));
#if Batchrendering 
		m_Renderer = std::unique_ptr<RGF::Renderer2D>(RGF::Renderer2D::Create(RenderingType::Batch));
#else
		m_Renderer = std::unique_ptr<RGF::Renderer2D>(RGF::Renderer2D::Create(RenderingType::Default));
#endif

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
		m_Camera = std::make_unique<Camera>();

		RGF_CORE_MSG("Creating the renderer!\n");
		m_Renderer->Init();

		RGF_CORE_TRACE("RGF application created!\n");
		RGF_CORE_TRACE("Time took to init application: %fms\n", m_AppTimer.GetElapsedMillis());


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

		Texture* test = nullptr;
		RGF::TextureParameters params;
		params.Filter = RGF::TextureFilter::Nearest;
		params.Format = RGF::TextureFormat::RGBA;
		params.Wrap = RGF::TextureWrap::Repeat;


		test = Texture::Create(0, 0, params);
		test->Bind();
		test->LoadTexture("res/graphics/sprite.png");

		Shader* shader = nullptr;
		shader = ShaderGenerator::GetInstance()->TexturedShader();
		shader->Bind();


		shader->SetUniform1i("u_TextureSampler", 0);
#if Batchrendering
		std::vector<BatchedSprite*> sprites;
#else
		std::vector<Sprite*> sprites;

#endif
#if 1
		for (float y = 0.0f; y < 9.0f; y += 0.14f) {
			for (float x = 0.0f; x < 16.0f; x += 0.14f) {
			sprites.push_back(new
#if Batchrendering
				BatchedSprite
#else
				Sprite
#endif
				({ 1-x, 1-y, 0.0f }, { 0.08f, 0.08f, 1.0f }, {Random::GetRandomInRange(1.0f, 1.0f), 0.0f, 1.0f, 1.0f}
#if !Batchrendering
				, shader
#endif

				));
		}
	}

#endif



		float xpos=0.0f;
		float ypos = 0.0f;
		RGF_CORE_MSG("Sprites: %d\n", sprites.size());
		while (m_IsRunning) {
#ifndef RGF_DISTRIBUTE
			m_EngineEditorLayer->GameView->ViewportFBO->Bind();
#endif
			m_Renderer->Clear();
			test->Bind();


			if (m_AppTimer.GetElapsedMillis() - UpdateTimer > UpdateTick) {
				Updates++;
				UpdateTimer += UpdateTick;
			}
			Frames++;

			for (Layer* layer : m_LayerStack.GetLayerStack()) {
				layer->OnUpdate();
			}
			if (Input::IsKeyDown(RGF_KEY_A)) {
				xpos-= 0.1f;
			}else if (Input::IsKeyDown(RGF_KEY_D)) {
				xpos+=0.1f;
			}


			if (Input::IsKeyDown(RGF_KEY_W)) {
				ypos-=0.1f;
			}else if (Input::IsKeyDown(RGF_KEY_S)) {
				ypos+= 0.1f;
			}
			//m_Camera->SetPosition({ xpos, ypos, 0.0f });
			m_Camera->Update();
			m_Renderer->Start();
			for (int i = 0; i < sprites.size(); i++) {
				m_Renderer->Submit(sprites[i]);
			}
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