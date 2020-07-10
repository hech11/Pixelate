#include "EditorLayer.h"

#include <RetroGF/Audio/Audio.h>
#include <RetroGF/Physics/Physics.h>

#include <RetroGF/Core/Input.h>
#include <RetroGF/Core/KeyCodes.h>
#include <RetroGF/Core/Application.h>

#include <RetroGF/Rendering/Renderer2D.h>
#include <RetroGF/Rendering/RenderCommand.h>

#include <Imgui/imgui.h>

#include <functional>
#include <Glm/gtc/type_ptr.hpp>

#include "../vendor/NativeFileDialog/src/include/nfd.h"
#include <RetroGF/Rendering/RendererAPI.h>


namespace RGF {

	void EditorLayer::Init() {

		Application::GetApp().GetWindow().SetTitle("Retro-Editor");
		Ref<FrameBuffer> m_ViewportFramebuffer;
		m_ViewportPanel = CreateScoped<EditorViewportPanel>();


		SpritePosition = { 0.0f, 0.0f, 0.0f };
		SpriteSize = { 1.0f, 1.0f, 1.0f };
		SpriteColor = { 1.00, 1.0f, 1.0f, 1.0f };
		

		LoadedFromFilepath = Texture::Create("assets/graphics/TestSpritesheet.png");
		GeneratedTexture = Texture::Create(1, 1);
		unsigned int data = 0xffA0Ef22;
		GeneratedTexture->SetData(&data, sizeof(unsigned int));

		SmileySprite = TextureBounds::Create(LoadedFromFilepath, { 0, 0, 16 ,16 });
		GaspSprite = TextureBounds::Create(LoadedFromFilepath, { 16, 0, 16 ,16 });

		particleProps.VelocityVariation1 = { -2.0f, -2.0f };
		particleProps.VelocityVariation2 = { 2.0f, 2.0f };
		particleProps.LifeTime = 1.0f;
		particleProps.SizeBegin = 1.5f;
		particleProps.SizeVariation = 0.0f;
		particleProps.SizeEnd = 0.0f;
		particleProps.ColorBegin = { 1.0f, 1.0f, 1.0f, 1.0f };
		particleProps.ColorEnd = { 0.0f, 0.0f, 1.0f , 0.0f };

		toneSFX = Audio::CreateAudioSource("assets/audio/tone-ogg.ogg");
		sameToneSfx = Audio::CreateAudioSource("assets/audio/tone-ogg.ogg");


		b2World* world = (b2World*)Physics::World();


		// Creating rigid bodies and setting their properties
		RigidBody::RigidBodyDef def1, def2;


		// the floor props
		def1.Type = RigidBody::BodyType::Static;
		def1.StartPosition = { 0.0f, -2.0f };

		// the player props
		def2.StartPosition = { 0.0f, 4.0f };
		def2.Type = RigidBody::BodyType::Dynamic;

		// create the bodies
		FloorRigidbody = CreateScoped<RigidBody>(def1);
		PlayerRigidbody = CreateScoped<RigidBody>(def2);

		// add colliders to the rigid bodies
		BoxColliderDef floorColliderDef, playerColliderDef;
		floorColliderDef.Size = { 5, 1 };

		BoxCollider floorCollider(floorColliderDef);
		FloorRigidbody->AddCollider(&floorCollider);


		// for a test I added two collision boxes to the player's rigid body. One of the boxes is offset
		playerColliderDef.Size = { 1, 1 };
		BoxCollider PlayerCollision(playerColliderDef);
		playerColliderDef.Centre = { 1, 0 };
		BoxCollider PlayerCollision2(playerColliderDef);

		PlayerRigidbody->AddCollider(&PlayerCollision);
		PlayerRigidbody->AddCollider(&PlayerCollision2);


		// testing entt
		m_TestEntity = m_Reg.create();
		m_Reg.emplace<Value>(m_TestEntity, 0, 5);
	}


	void EditorLayer::ShutDown() {}

	void EditorLayer::OnUpdate(float dt) {

		using namespace RGF;

		{
			m_ViewportPanel->OnUpdate(dt);

			if (m_ViewportPanel->IsHovered()) {
				if (Input::IsMouseButtonDown(0)) {
					float x = m_ViewportPanel->GetCamera()->GetMousePositionRelativeToViewportPanel().x;
					float y = m_ViewportPanel->GetCamera()->GetMousePositionRelativeToViewportPanel().y;



					float width = m_ViewportPanel->GetViewportSize().x;
					float height = m_ViewportPanel->GetViewportSize().y;

					auto bounds = m_ViewportPanel->GetCamera()->GetBounds();
					auto pos = m_ViewportPanel->GetCamera()->GetCamera().GetPos();
					x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
					y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
					particleProps.Position = { x + pos.x, y + pos.y };


					for (unsigned int i = 0; i < particleSystem.InitData.SpawnRate; i++) {
						particleSystem.Emit(particleProps);
					}

				}
			}

			if (Input::IsKeyDown(RGF_KEY_D)) {
				PlayerRigidbody->SetLinearVelocity({ VelocitySpeed, PlayerRigidbody->GetLinearVelocity().y });
			}
			if (Input::IsKeyDown(RGF_KEY_A)) {
				PlayerRigidbody->SetLinearVelocity({ -VelocitySpeed, PlayerRigidbody->GetLinearVelocity().y });
			}


			particleSystem.OnUpdate(dt);
			Audio::Update();

			Update(m_Reg);
		}

		{

			m_ViewportPanel->DrawToViewport();

			RenderCommand::Clear();
			RenderCommand::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

			Renderer2D::ResetStatistics();

			Renderer2D::BeginScene(&m_ViewportPanel->GetCamera()->GetCamera());

			Renderer2D::DrawSprite({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { .5f, .5f, .5f, 1.0f });
			Renderer2D::DrawSprite({ 2.0f, 1.0f, 1.0f }, 50.0f, { .5f, 5.f, 1.0f }, SpriteColor);
			Renderer2D::DrawSprite({ 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, LoadedFromFilepath, { 1.0f, 1.0f, 1.0f, 1.0f });
			Renderer2D::DrawSprite({ 0.0f, -2.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, SmileySprite, { 1.0f, 1.0f, 1.0f, 1.0f });
			Renderer2D::DrawSprite({ 1.0f, -2.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, GaspSprite, { 0.5f, 1.0f, 1.0f, 0.5f });
			Renderer2D::DrawSprite({ -1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, LoadedFromFilepath, { 1.0f, 1.0f, 1.0f, 0.5f });
			Renderer2D::DrawSprite({ -2.0f, 1.0f, 0.0f }, 25.0f, { 1.0f, 1.0f, 1.0f }, LoadedFromFilepath, { 1.0f, 1.0f, 1.0f, 0.5f });
			Renderer2D::DrawSprite({ 2.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, GeneratedTexture, { 1.0f, 1.0f, 1.0f, 1.0f });
			Renderer2D::DrawSprite({ 2.0f, 0.0f, 0.0f }, { 3.0f, 0.5f, 1.0f }, GeneratedTexture, { 0.4f, 0.8f, 0.2f, 0.75f });
			Renderer2D::DrawSprite({ 2.0f, 2.0f, 0.0f }, 75.0f, { 1.0f, 1.0f, 1.0f }, GeneratedTexture, { 1.0f, 1.0f, 1.0f, 1.0f });


			Renderer2D::DrawSprite(FloorRigidbody->GetPosition(), Rotation, { 5.0f, 1.0f, 1.0f }, { .75f,0.25f,0.4f, 1.0f });
			Renderer2D::DrawSprite(PlayerRigidbody->GetPosition(), 0.0f, { 1.0f, 1.0f, 1.0f }, SpriteColor);


			particleSystem.OnRender();
			Renderer2D::EndScene();

			Physics::DrawDebugObjects();

			m_ViewportPanel->FinishDrawing();

		}
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e) {
		if (e.GetKeyCode() == RGF_KEY_P && e.GetRepeatCount() == 0) {
			toneSFX->Play();
			return true;
		}

		if (e.GetKeyCode() == RGF_KEY_O && e.GetRepeatCount() == 0) {
			sameToneSfx->Play();
			return true;
		}

		if (e.GetKeyCode() == RGF_KEY_S && e.GetRepeatCount() == 0) {
			toneSFX->Stop();
			return true;

		}
		if (e.GetKeyCode() == RGF_KEY_I && e.GetRepeatCount() == 0) {
			toneSFX->Pause();
			return true;

		}

		if (e.GetKeyCode() == RGF_KEY_L && e.GetRepeatCount() == 0) {
			static bool loop = false;
			loop = !loop;
			RGF_TRACE("Looping is %d\n", loop);
			toneSFX->SetLooping(loop);
			return true;

		}


		return false;
	}

	void EditorLayer::OnEvent(Event& e) {
		m_ViewportPanel->OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&EditorLayer::OnKeyPressedEvent, this, std::placeholders::_1));

	}


	void EditorLayer::OnImguiRender() {
#ifdef RGF_USE_IMGUI
		using namespace RGF;




		
/////////////////////// The dockspace and menu bar //////////////////////////////////
		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;



		// This is for the menu bar.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) // This is settings for the dockspace.
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// To render the menu hotbar
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("MenuBar", (bool*)true, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File"))
			{
				ImGui::Separator();
				if (ImGui::MenuItem("Exit", "")) {
					Application::GetApp().Quit();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();
/////////////////////////////////////////////////////////////////////////////////////








		/*
		static bool showDemoWindow = true;
		ImGui::ShowDemoWindow(&showDemoWindow);
		*/


		ImGui::Begin("Renderer stats");
		ImGui::Text("DrawCalls: %d", Renderer2D::GetStats().DrawCalls);
		ImGui::Text("Max Sprites: %d", Renderer2D::GetStats().MaxSprites);
		ImGui::Text("Max VBO size: %d", Renderer2D::GetStats().MaxVertexBufferSize);
		ImGui::Text("Max IBO size: %d", Renderer2D::GetStats().MaxIndexBuferSize);
		ImGui::Text("Vertex ptr size: %d", Renderer2D::GetStats().VertexSize);
		ImGui::Text("IndexCount: %d", Renderer2D::GetStats().IndexCount);
		ImGui::End();

		ImGui::Begin("Sprite props");
		ImGui::SliderFloat("Sprite MoveSpeed", &VelocitySpeed, 0.0f, 4.0f, "%.2f");
		auto pos = PlayerRigidbody->GetPosition();
		ImGui::SliderFloat3("Sprite Position old", glm::value_ptr(pos), -10.0f, 10.0f, "%.2f");
		ImGui::SliderFloat3("Sprite Position new", glm::value_ptr(SpritePosition), -10.0f, 10.0f, "%.2f");
		ImGui::SliderFloat("Sprite Rotation", &Rotation, -360.0f, 360.0f, "%.2f");
		ImGui::SliderFloat3("Sprite Size", glm::value_ptr(SpriteSize), -10.0f, 10.0f, "%.2f");
		ImGui::ColorPicker4("Sprite Color", glm::value_ptr(SpriteColor));
		ImGui::Image((void*)LoadedFromFilepath->GetHandleID(), { 150, 150 }, { 0, 1 }, { 1, 0 });
		ImGui::SameLine();
		if (ImGui::SmallButton("...")) {
			nfdchar_t* outPath = NULL;
			nfdresult_t result = NFD_OpenDialog("png", NULL, &outPath);

			if (result == NFD_OKAY) {
				puts("Success!");
				LoadedFromFilepath->SetData(outPath);
				puts(outPath);
				free(outPath);
			}
			else if (result == NFD_CANCEL) {
				puts("User pressed cancel.");
			}
			else {
				printf("Error: %s\n", NFD_GetError());
			}

		}
		ImGui::End();

		ImGui::Begin("Physics");
		unsigned int flags = 0;
		static bool physicsDebugButton = false;
		static bool shapeBit = false;
		static bool jointBit = false;
		static bool aabbBit = false;
		static bool pairBit = false;
		static bool centerOfMassBit = false;

		ImGui::Checkbox("Physics debug", &physicsDebugButton);
		ImGui::Checkbox("shapeBit", &shapeBit);
		ImGui::Checkbox("jointBit", &jointBit);
		ImGui::Checkbox("aabbBit", &aabbBit);
		ImGui::Checkbox("pairBit", &pairBit);
		ImGui::Checkbox("centerOfMassBit", &centerOfMassBit);
		Physics::GetDebug().ShouldDrawVisuals(physicsDebugButton);

		if (shapeBit)
			flags = flags | PhysicsDebugDraw::DrawFlag::shapeBit;

		if (jointBit)
			flags = flags | PhysicsDebugDraw::DrawFlag::jointBit;

		if (aabbBit)
			flags = flags | PhysicsDebugDraw::DrawFlag::aabbBit;

		if (pairBit)
			flags = flags | PhysicsDebugDraw::DrawFlag::pairBit;

		if (centerOfMassBit)
			flags = flags | PhysicsDebugDraw::DrawFlag::centerOfMassBit;



		Physics::GetDebug().SetDrawFlag(flags);
		ImGui::End();


		ImGui::Begin("Application");
		static float time = 0.0f;
		static std::string ts = "Timestep: " + std::to_string(Application::GetApp().GetTimestep() * 1000.0f) + "(ms)";
		if (Application::GetApp().GetTime().GetElapsedSeconds() - time > 1.0f) {
			time += 1.0f;
			ts = "Timestep: " + std::to_string(Application::GetApp().GetTimestep() * 1000.0f) + "(ms)";
		}
		std::string context = "Context: " + RenderCommand::GetCaps().ContextName;
		std::string info = "GPU Info: " + RenderCommand::GetCaps().RendererName;
		std::string version = "GPU Version: " + RenderCommand::GetCaps().Version;
		ImGui::Text(ts.c_str());
		ImGui::Text(context.c_str());
		ImGui::Text(info.c_str());
		ImGui::Text(version.c_str());

		ImGui::End();



		m_ViewportPanel->OnImguiRender();

#endif


	}

}