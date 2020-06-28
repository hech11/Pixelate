#include "RGFpch.h"
#include "Application.h"


#include "RetroGF/Platform/Windows/WindowsWindow.h"

#include <RetroGF/Rendering/API/Buffer.h>
#include <RetroGF/Rendering/API/VertexArray.h>
#include <RetroGF/Rendering/API/Texture.h>


#include <GLM/glm/gtc/matrix_transform.hpp>
#include <RetroGF/Utility/Random.h>

#include <RetroGF/Rendering/Renderer2D.h>
#include <RetroGF/Rendering/RenderCommand.h>

#include "RetroGF/Platform/OpenGL/GLCommon.h"

#include "RetroGF/Core/Input.h"
#include "RetroGF/Core/KeyCodes.h"
#include "RetroGF/Core/MouseButtonCodes.h"

#include "RetroGF/Audio/Audio.h"


// This is temp. This is used to test out box2d
#include <box2d/box2d.h>
#include "RetroGF/Physics/PhysicsDebugDraw.h"


namespace RGF {




	Application* Application::s_Instance = nullptr;

	static Scoped<b2World> s_PhysicsWorld;
	static b2Body* s_TestBody;
	static b2Fixture* s_TestFixture;

	static float fixedTimeStep = 1 / 20.0f;
	static int velocityIterations = 8;
	static int positionIterations = 3;

	Application::Application() {
		RGF_PROFILE_FUNCTION();
		s_Instance = this;
		b2Vec2 grav = { 0.0f, -9.8f };
		s_PhysicsWorld = CreateScoped<b2World>(grav);

		m_Window = Scoped<WindowImpl>(WindowImpl::Create({960,540}));
		// Bind the "OnEvent" to the function pointer in "WindowImpl.h"
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

#ifdef RGF_USE_IMGUI
		m_ImguiLayer = new ImguiLayer;
#endif




		RGF_CORE_MSG("Creating the renderer!\n");
		Renderer2D::Init();

		RGF_CORE_MSG("initializing the audio!\n");
		Audio::Init();

		RGF_CORE_TRACE("RGF application created!\n");
		RGF_CORE_TRACE("Time took to init application: %fms\n", m_AppTimer.GetElapsedMillis());




		// This is to test box2d
		{
			b2BodyDef bDef;
			bDef.type = b2_staticBody;
			bDef.position.Set(0, 0);
			bDef.angle = 0.0f;
			s_TestBody = s_PhysicsWorld->CreateBody(&bDef);

			b2PolygonShape boxShape;
			boxShape.SetAsBox(3, 0.63);

			b2FixtureDef fDef;
			fDef.shape = &boxShape;
			fDef.density = 1;
			s_TestFixture = s_TestBody->CreateFixture(&fDef);

			s_PhysicsWorld->SetDebugDraw(&m_PhysicsDebugDraw);
			m_PhysicsDebugDraw.SetFlags(b2Draw::e_shapeBit);
		}
	}
	Application::~Application() {
		RGF_PROFILE_FUNCTION();
		Renderer2D::ShutDown();
	}


	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);

	}
	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}


	// Is bound to the function pointer in "m_Window". This function will be called when a event happens.
	void Application::OnEvent(Event& e) {
		RGF_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);


		// Checks if the event was a "WindowCloseEvent". If it was the event, call the "OnWindowClose" function.
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

		for (Layer* layer : m_LayerStack.GetLayerStack()) {
			layer->OnEvent(e);
			if (e.Handled)
				break;
		}
	}


	void Application::Run() {
		RGF_PROFILE_FUNCTION();

		//TODO: Need to refactor the game loop for a better FPS counter
		float FPS = 60.0f;
		float UpdateTimer = 0.0f;
		float UpdateTick = 1.0f / FPS;
		float Time = 0.0f;

		float LastTime = 0.0f;

		while (m_IsRunning) {
			RGF_PROFILE_SCOPE("Application::Run::m_IsRunning::Loop");
			RenderCommand::Clear();
			float time = m_AppTimer.GetElapsedSeconds();
			float timeStep = time - LastTime;
			LastTime = time;



			for (Layer* layer : m_LayerStack.GetLayerStack()) {

				RGF_PROFILE_SCOPE("Application::Layer::" + layer->GetName() + "::OnUpdate");

				layer->OnUpdate(timeStep);
			}

			s_PhysicsWorld->Step(fixedTimeStep, velocityIterations, positionIterations);

			// TODO: This is temp. When I decide to start developing the editor, this code will be removed
			Renderer2D::BeginScene(&m_PhysicsDebugDraw.GetCamera()->GetCamera());
			s_PhysicsWorld->DebugDraw();
			Renderer2D::EndScene();

#ifdef RGF_USE_IMGUI

			{
				RGF_PROFILE_SCOPE("Application::ImguiRender||EngineEditor");
				m_ImguiLayer->Start();
				for (Layer* layer : m_LayerStack.GetLayerStack()) {
					RGF_PROFILE_SCOPE("Application::Layer::" + layer->GetName() + "::OnImguiRender");
					layer->OnImguiRender();
				}
				m_ImguiLayer->End();
			}
#endif


			m_Window->OnUpdate();

			if (m_AppTimer.GetElapsedSeconds() - Time > 1.0f) {
				Time += 1.0f;
				RGF_CORE_MSG("%f: timestep\n", timeStep*1000.0f);
#ifdef RGF_DISTRIBUTE
				printf("%f: timestep\n", timeStep*1000.0f);
#endif
			}
		}

	}


	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_IsRunning = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e) {
		RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}


}