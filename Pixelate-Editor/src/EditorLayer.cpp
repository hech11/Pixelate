#include "EditorLayer.h"

#include <Pixelate/Audio/Audio.h>
#include <Pixelate/Physics/Physics.h>

#include <Pixelate/Core/Input.h>
#include <Pixelate/Core/KeyCodes.h>
#include <Pixelate/Core/Application.h>

#include <Pixelate/Rendering/Renderer2D.h>
#include <Pixelate/Rendering/RenderCommand.h>

#include <Pixelate/Scene/SceneManager.h>


#include <Imgui/imgui.h>
#include <Imgui/imgui_internal.h>

#include <functional>
#include <Glm/gtc/type_ptr.hpp>

#include <Pixelate/Rendering/RendererAPI.h>
#include <Pixelate/Core/MouseButtonCodes.h>
#include "glm/ext/quaternion_common.hpp"
#include "Pixelate/Imgui/ImGuizmo.h"
#include "glm/gtx/matrix_decompose.hpp"
#include "../../NativeFileDialog/src/include/nfd.h"

#include <Pixelate/Editor/EditorContentBrowser.h>
#include <Pixelate/Asset/AssetManager.h>

namespace Pixelate {


	static bool GoTo2ndClip = false;
	static bool GoTo1stClip = false;
	void EditorLayer::Init() {



		particleProps.VelocityVariation1 = { -2.0f, -2.0f };
		particleProps.VelocityVariation2 = { 2.0f, 2.0f };
		particleProps.LifeTime = 1.0f;
		particleProps.SizeBegin = 1.5f;
		particleProps.SizeVariation = 0.0f;
		particleProps.SizeEnd = 0.0f;
		particleProps.ColorBegin = { 1.0f, 1.0f, 1.0f, 1.0f };
		particleProps.ColorEnd = { 0.0f, 0.0f, 1.0f , 0.0f };


		//m_EditorScene = SceneManager::LoadScene("assets/scenes/PhysicsTests.PXScene");
		//m_EditorScene = SceneManager::LoadScene("assets/scenes/test123.PXScene");
		//m_EditorScene = SceneManager::LoadScene("assets/scenes/DefaultScene.PXScene");
		m_EditorScene = SceneManager::GenerateDefaultScene();


		auto& PanelManager = EditorPanelManager::Get();
		PanelManager.RegisterPanel("SceneHierarcy", m_SceneHierarcyPanel = CreateRef<EditorSceneHierarchyPanel>(m_EditorScene));
		PanelManager.RegisterPanel("TextureInspector" ,m_TextureInspector = CreateRef<EditorTextureInspector>(), false);
		PanelManager.RegisterPanel("ConsoleLog", m_ConsoleLog = CreateRef<EditorConsoleLogger>(true));
		PanelManager.RegisterPanel("ContentBrowser", CreateRef<EditorContentBrowser>());


		//m_EditorPanelManager->RegisterPanel(m_AnimatorPanel = CreateRef<EditorAnimationPanel>());

		// testing asset handles direcly

		assetHandleTestEntity = m_EditorScene->CreateEntity("Texture loaded via asset handle");
		assetHandleTestEntity.AddComponent<SpriteRendererComponent>();
		assetHandleTestEntity.GetComponent<SpriteRendererComponent>().Texture = AssetManager::GetAsset<Texture>((AssetHandle)18069192197029962527);
		assetHandleTestEntity.GetComponent<SpriteRendererComponent>().Rect = Rect({ 0, 0 }, {128, 128});



		// testing animation //
// 		animationTest = m_EditorScene->CreateEntity("Animation test entity");
// 		animationComponentTest = m_EditorScene->CreateEntity("Animator component test entity");
// 
// 		animationTest.AddComponent<SpriteRendererComponent>();
// 		animationTest.GetComponent<SpriteRendererComponent>().Texture = Texture::Create("assets/graphics/TestSpritesheet.png");
// 		animationTest.GetComponent<SpriteRendererComponent>().Rect = Rect({ 0, 0 }, { 128, 128 });
// 
// 		animationComponentTest.AddComponent<AnimatorComponent>();
// 
// 		animationComponentTest.AddComponent<SpriteRendererComponent>();
// 		animationComponentTest.GetComponent<SpriteRendererComponent>().Texture = animationTest.GetComponent<SpriteRendererComponent>().Texture;
// 		animationComponentTest.GetComponent<SpriteRendererComponent>().Rect = Rect({ 0, 0 }, { 128, 128 });
// 		animationComponentTest.GetComponent<TransformComponent>().SetPosition({ 1, 0, 0 });
// 
// 
// 		AnimationClip clip, clip2;
// 		AnimationFrame frame, frame2, frame3;
// 		AnimationFrame frame1, frame12, frame13;
// 
// 		frame.FrameRect = { {0, 0}, {16, 16} };
// 		frame.FrameTiming = 0.0f;
// 
// 		clip.AddFrame(frame);
// 
// 		frame2.FrameRect = { {0, 0}, {16, 16} };
// 		frame2.FrameTiming = 1.0f;
// 
// 		clip.AddFrame(frame2);
// 
// 		frame3.FrameRect = { {16, 0}, {16, 16} };
// 		frame3.FrameTiming = 2.0f;
// 
// 
// 		clip.AddFrame(frame3);
// 
// 
// 		anim.AddClip(clip);
// 
// 
// 
// 		frame1.FrameRect = { {0, 0}, {16, 16} };
// 		frame1.FrameTiming = 0.0f;
// 
// 		clip2.AddFrame(frame1);
// 
// 		frame12.FrameRect = { {0, 0}, {16, 16} };
// 		frame12.FrameTiming = 1.0f;
// 
// 		clip2.AddFrame(frame12);
// 
// 		frame13.FrameRect = { {0, 16}, {16, 16} };
// 		frame13.FrameTiming = 2.0f;
// 
// 
// 		clip2.AddFrame(frame13);
// 
// 
// 		anim.AddClip(clip2);
// 
// 
// 		anim.AddTransition({ clip, clip2, []() {return GoTo2ndClip; } });
// 		anim.AddTransition({ clip2, clip, []() {return GoTo1stClip; } });


		//Setting up both scene and game viewport panels
		FramebufferSpecs ViewportSpecs;
		ViewportSpecs.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		ViewportSpecs.Width = 960;
		ViewportSpecs.Height = 540;

		FramebufferSpecs sceneSpecs;
		sceneSpecs.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INT, FramebufferTextureFormat::Depth };
		sceneSpecs.Width = 960;
		sceneSpecs.Height = 540;





		m_SceneViewportFramebuffer = Framebuffer::Create(sceneSpecs);
		m_GameViewportFramebuffer = Framebuffer::Create(ViewportSpecs);

		FramebufferPool::Add(m_SceneViewportFramebuffer);
		FramebufferPool::Add(m_GameViewportFramebuffer);

		ViewportPanelProps props;
		props.Position = &m_SceneViewportPanelPosition;
		props.Size = &m_SceneViewportPanelSize;

		m_EditorCamera = CreateRef<EditorCamera>(16.0f / 9.0f, props);
		m_EditorCamera->SetOrthographicSize(5.0f);


		Renderer2D::SetBoundingBox(true);

		m_Gizmo = ImGuizmo::TRANSLATE;


		FileSystem::StartWatching(); // Should move this when projects are introduced.

	}


	void EditorLayer::ShutDown() {
		FileSystem::StopWatching(); // Should move this when projects are introduced.
	}

	void EditorLayer::OnUpdate(float dt) {
		auto& PanelManager = EditorPanelManager::Get();


		if (m_IsSceneViewportFocused) {
			m_EditorCamera->OnUpdate(dt);
		}
		if (m_SceneViewportSize != m_SceneViewportPanelSize) {
			m_SceneViewportFramebuffer->Resize(m_SceneViewportPanelSize.x, m_SceneViewportPanelSize.y);
			m_SceneViewportSize = m_SceneViewportPanelSize;
			m_EditorCamera->Resize(m_SceneViewportPanelSize.x / m_SceneViewportPanelSize.y);
		}

		if (m_GameViewportSize != m_GameViewportPanelSize) {
			m_GameViewportFramebuffer->Resize(m_GameViewportPanelSize.x, m_GameViewportPanelSize.y);
			m_GameViewportSize = m_GameViewportPanelSize;
		}

		m_EditorScene->SetGameViewport(m_GameViewportPanelSize.x, m_GameViewportPanelSize.y);

		// Render scene viewport to the its fbo

		m_SceneViewportFramebuffer->Bind();

		RenderCommand::Clear();
		m_SceneViewportFramebuffer->ClearColorAttachment(1, -1);

		m_EditorScene->OnUpdate(dt, m_EditorCamera, m_SceneHierarcyPanel->CurrentlySelectedEntity(),m_SceneHierarcyPanel->HasAnEntitySelected());

		//anim.Update(dt);
		PanelManager.OnUpdate(dt);


		//Renderer2D::BeginScene(m_EditorCamera.get());
		
		//animationTest.GetComponent<SpriteRendererComponent>().Rect = anim.GetCurrentClip().GetCurrentFrameRect();
		//Renderer2D::DrawSprite(animationTest.GetComponent<TransformComponent>(), animationTest.GetComponent<SpriteRendererComponent>());


		//Renderer2D::EndScene();


	

		m_SceneViewportFramebuffer->Unbind();

		// Render game viewport to the its fbo
		m_GameViewportFramebuffer->Bind();


		m_EditorScene->OnGameViewportRender();
		m_GameViewportFramebuffer->Unbind();

		if (m_SceneState == SceneState::Play) {
			m_EditorScene->OnRuntimeUpdate(dt);
			Audio::Update();
			bool updateHierarcy = false;
			SceneManager::LoadAnyQueuedScenes(m_EditorScene, &updateHierarcy);
			if (updateHierarcy)
				m_SceneHierarcyPanel->SetSceneContext(m_EditorScene);
			
		}

	
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e) {

		if (e.GetKeyCode() == (int)KeyCode::LeftControl) {
			if (e.GetKeyCode() == (int)KeyCode::O) {
				OpenScene();
			}

		}

		bool lCtrl = Input::IsKeyDown(KeyCode::LeftControl);
		bool lShift = Input::IsKeyDown(KeyCode::LeftShift);

		switch ((KeyCode)e.GetKeyCode())
		{
			case KeyCode::O:
			{
				if (lCtrl) {
					OpenScene();
					break;
				}
			}

			case KeyCode::S:
			{
				if (lCtrl && lShift) {
					SaveScene();
					break;
				}
			}

			case KeyCode::Q: {
				if (!ImGuizmo::IsUsing()) {
					m_Gizmo = ImGuizmo::TRANSLATE;
					break;
				}
			}

			case KeyCode::W: {
				if (!ImGuizmo::IsUsing()) {
					m_Gizmo = ImGuizmo::ROTATE;
					break;
				}
			}

			case KeyCode::E: {
				if (!ImGuizmo::IsUsing()) {
					m_Gizmo = ImGuizmo::SCALE;
					break;
				}
			}


		}

		return false;
	}


	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		auto& PanelManager = EditorPanelManager::Get();

// 		if (e.GetButton() == (int)MouseButton::Left && e.GetRepeatCount() == 0 && m_IsSceneViewportHovered) {
// 
// 			auto allSpriteEntities = m_EditorScene->GetAllEntitiesWith<SpriteRendererComponent>();
// 			bool intersects = false;
// 			for (auto s : allSpriteEntities) {
// 				Entity e = { s, m_EditorScene.get() };
// 				auto& transform = e.GetComponent<TransformComponent>();
// 				auto [position, rot, scale] = transform.DecomposeTransform();
// 
// 				intersects = m_EditorCamera->IsIntersecting(position, scale);
// 				if (intersects && (!ImGuizmo::IsOver() || !ImGuizmo::IsUsing())) {
// 					PanelManager.SetSelectedEntity({s, m_EditorScene.get()});
// 					break;
// 				} 
// 			}
// 			if (!intersects && !ImGuizmo::IsOver()) {
// 				PanelManager.SetSelectedEntity({});
// 				//m_AnimatorPanel->SetEntityContext();
// 			}
// 
// 		}

		if (e.GetButton() == (int)MouseButton::Left && e.GetRepeatCount() == 0 && m_IsSceneViewportHovered) {

			auto [mx, my] = ImGui::GetMousePos();
			mx -= m_SceneViewportPanelPosition.x;
			my -= m_SceneViewportPanelPosition.y;
			my = m_SceneViewportPanelSize.y - my;
			m_SceneViewportFramebuffer->Bind();
			int px = m_SceneViewportFramebuffer->ReadPixel(1, (int)mx, (int)my);
			PX_CORE_MSG("Entity handle: %d\n", px);
			if (px != -1 && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver()) {
				Entity e = { (entt::entity)px, m_EditorScene.get() };
				EditorPanelManager::Get().SetSelectedEntity(e);
			}
			else if(!ImGuizmo::IsUsing() && !ImGuizmo::IsOver()){
				EditorPanelManager::Get().SetSelectedEntity({});
			}
			m_SceneViewportFramebuffer->Unbind();

		}

		return false;
	}

	void EditorLayer::OnScenePlay() {
		m_SceneState = SceneState::Play;

		SceneManager::StoreSceneBeforePlay();
		SceneManager::SetPlayMode(true);

		m_EditorScene->OnRuntimeStart();


	}

	void EditorLayer::OnSceneStop() {
		auto& PanelManager = EditorPanelManager::Get();
		m_SceneState = SceneState::Edit;
		m_EditorScene->OnRuntimeStop();

		SceneManager::SetPlayMode(false);
		m_EditorScene = SceneManager::LoadStoredSceneAfterRuntimeStop();


		PanelManager.SetSceneContext(m_EditorScene);

	}

	void EditorLayer::OpenScene()
	{
		auto& PanelManager = EditorPanelManager::Get();

		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_OpenDialog("PXScene", NULL, &outPath);
		if (result == NFD_OKAY) {
			PX_CORE_TRACE("Success!");

			m_EditorScene = SceneManager::LoadScene(outPath);
			PanelManager.SetSceneContext(m_EditorScene);


			free(outPath);
		}
		else if (result == NFD_CANCEL) {
			PX_CORE_MSG("User pressed cancel.\n");
		}
		else {
			PX_CORE_ERROR("Error: %s\n", NFD_GetError());
		}
	}

	void EditorLayer::SaveScene()
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_SaveDialog("PXScene", NULL, &outPath);
		if (result == NFD_OKAY) {
			puts("Success!");

			std::string filepath = std::string(outPath);
			std::string name;
			size_t slashPos = filepath.rfind("\\") + 1;

			name.append(filepath.substr(slashPos, filepath.length()));
			m_EditorScene->SetName(name);

			filepath.append(".PXScene");

			SceneManager::SaveScene(m_EditorScene, filepath);

			free(outPath);
		}
		else if (result == NFD_CANCEL) {
			PX_CORE_MSG("User pressed cancel.\n");
		}
		else {
			PX_CORE_ERROR("Error: %s\n", NFD_GetError());
		}

	}

	void EditorLayer::OnEvent(Event& e) {
		if(m_IsSceneViewportFocused)
			m_EditorCamera->OnEvent(e);

		auto& PanelManager = EditorPanelManager::Get();
		PanelManager.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&EditorLayer::OnKeyPressedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&EditorLayer::OnMouseButtonPressed, this, std::placeholders::_1));


	
	}


	void EditorLayer::OnImguiRender() {
#ifdef PX_USE_IMGUI
		using namespace Pixelate;
		auto& PanelManager = EditorPanelManager::Get();





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
				if (ImGui::MenuItem("Open Project", "")) {
					m_OpeningModal = true;
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Open Scene...", "Ctr+O")) {
					OpenScene();
				}

				if (ImGui::MenuItem("Save Scene...", "Ctr+Shift+S")) {
					SaveScene();

				}
				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "")) {
					Application::GetApp().Quit();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Reload CSharp Assembly", "")) {
					//ScriptingMaster::ReloadAssembly();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings")) {
				if (ImGui::MenuItem("Scenes", "")) {
					m_OpenSceneManagerPanel = true;
				}
				if (ImGui::MenuItem("Physics", "")) {
					m_OpenPhysicsPanel = true;
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();
/////////////////////////////////////////////////////////////////////////////////////



// 		static bool showDemoWindow = true;
// 		ImGui::ShowDemoWindow(&showDemoWindow);

	

		ImGui::Begin("Animation test");
		if (ImGui::Button("Play2ndClip")) {
			GoTo2ndClip = true;
			GoTo1stClip = false;
		}
		if (ImGui::Button("Play1stClip")) {
			GoTo2ndClip = false;
			GoTo1stClip = true;
		}
		ImGui::End();


		ImGui::Begin("Renderer stats");
		ImGui::Text("DrawCalls: %d", Renderer2D::GetStats().DrawCalls);
		ImGui::Text("Max Sprites: %d", Renderer2D::GetStats().MaxSprites);
		ImGui::Text("Max VBO size: %d", Renderer2D::GetStats().MaxVertexBufferSize);
		ImGui::Text("Max IBO size: %d", Renderer2D::GetStats().MaxIndexBuferSize);
		ImGui::Text("Vertex ptr size: %d", Renderer2D::GetStats().VertexSize);
		ImGui::Text("IndexCount: %d", Renderer2D::GetStats().IndexCount);
		if(ImGui::Button("Draw bounding boxes")) {
			Renderer2D::SetBoundingBox(!Renderer2D::ShouldDrawBoundingBox());
		}
		std::string pos = std::to_string(m_EditorCamera->GetPos().x ) + ", " + std::to_string(m_EditorCamera->GetPos().y);
		std::string rot = std::to_string(m_EditorCamera->GetAngle());
		std::string orthoSize = std::to_string(m_EditorCamera->GetOrthographicSize());

		ImGui::Text("Camera Editor transform");
		ImGui::Text("Position: ");
		ImGui::SameLine();
		ImGui::Text(pos.c_str());
		ImGui::Text("Rotation (Z): ");
		ImGui::SameLine();
		ImGui::Text(rot.c_str());
		ImGui::Text("OrthoSize: ");
		ImGui::SameLine();
		ImGui::Text(orthoSize.c_str());
		ImGui::End();

		ImGui::Begin("Application");
		static float time = 0.0f;
		static std::string ts = "Timestep: " + std::to_string(Application::GetApp().GetTimestep() * 1000.0f) + "(ms)";
		ts = "Timestep: " + std::to_string(Application::GetApp().GetTimestep() * 1000.0f) + "(ms)";
		
		std::string context = "Context: " + RenderCommand::GetAPIData().ContextName;
		std::string info = "GPU Info: " + RenderCommand::GetAPIData().RendererName;
		std::string version = "GPU Version: " + RenderCommand::GetAPIData().Version;
		ImGui::Text(ts.c_str());
		ImGui::Text(context.c_str());
		ImGui::Text(info.c_str());
		ImGui::Text(version.c_str());
		

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 4));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));


		ImGui::Begin("Toolbar" , 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
		
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();


		ImGui::SetCursorPos({ ImGui::GetWindowSize().x / 2  + -161, ImGui::GetWindowSize().y /2  +  -23});
		if (m_SceneState == SceneState::Play) {
			if (ImGui::Button("Stop", { 145, 42 })) {
				OnSceneStop();
			}
		} else if (m_SceneState == SceneState::Edit) {
			if (ImGui::Button("Play", { 145, 42 })) {
				OnScenePlay();
			}
		}

		ImGui::SetCursorPos({ ImGui::GetWindowSize().x / 2 + -12, ImGui::GetWindowSize().y / 2 + -23 });

		if (ImGui::Button("Pause", { 145, 42 })) {
		}
		
		ImGui::Separator();
		ImGui::End();

		PanelManager.OnImguiRender();
		AssetManager::OnImguiRender(true);



		// scene viewport panel
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Scene Viewport", 0, ImGuiWindowFlags_NoCollapse);


		static glm::vec4 sceneColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		ImGui::PushItemWidth(250);
		ImGui::ColorEdit4("Scene color", glm::value_ptr(sceneColor));
		float ColorRectYSize = ImGui::GetItemRectSize().y;
		ImGui::PopItemWidth();


		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y + ColorRectYSize;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;

		ImRect dragAreaRect = { vMin , vMax };
		auto sceneViewportID = ImGui::GetID("Scene Viewport");

		
		if (ImGui::BeginDragDropTargetCustom(dragAreaRect, sceneViewportID)) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetPayload"))
			{

				
				AssetMetadata& metadata = *(AssetMetadata*)payload->Data;
				std::filesystem::path filepath = AssetManager::GetFilePath(metadata);
				


				PX_CORE_MSG("Scene viewport recieved '%s'!\n", filepath.c_str());

				switch (metadata.Type)
				{
					case AssetType::Scene: 
					{
						m_EditorScene = SceneManager::LoadScene(filepath.string());
						PanelManager.SetSceneContext(m_EditorScene);
						break;
					}
					case AssetType::Texture:
					{
						Entity entity = m_EditorScene->CreateEntity(filepath.stem().string());
						entity.GetComponent<TransformComponent>().SetPosition(m_EditorCamera->GetPos());
						SpriteRendererComponent& comp = entity.AddComponent<SpriteRendererComponent>();
						comp.Texture = AssetManager::GetAsset<Texture>(metadata.Handle);
						comp.Rect = { {0, 0}, {comp.Texture->GetWidth(), comp.Texture->GetHeight()} };
						break;
					}
					case AssetType::Audio:
					{
						Entity entity = m_EditorScene->CreateEntity(filepath.stem().string());
						entity.GetComponent<TransformComponent>().SetPosition(m_EditorCamera->GetPos());
						AudioSourceComponent& comp = entity.AddComponent<AudioSourceComponent>();
						comp.Source = AssetManager::GetAsset<AudioSource>(metadata.Handle);
						comp.FilePath = metadata.Filepath.string();
						break;
					}

				}

			}
			ImGui::EndDragDropTarget();
		}



		RenderCommand::SetClearColor(sceneColor.r, sceneColor.g, sceneColor.b, sceneColor.a);
		ImGui::Separator();

		m_IsSceneViewportHovered = ImGui::IsWindowHovered();
		m_IsSceneViewportFocused = ImGui::IsWindowFocused();

		if (m_IsSceneViewportHovered && (Input::IsMouseButtonDown(MouseButton::Right) || Input::IsMouseButtonDown(MouseButton::Middle))) {
			m_IsSceneViewportFocused = true;
			ImGui::SetWindowFocus();
		}


		Application::GetApp().GetImguiLayer().ShouldBlockEvents(false);

		m_SceneViewportPanelPosition = *((glm::vec2*) & ImGui::GetWindowPos());
		m_SceneViewportPanelPosition += *((glm::vec2*) & ImGui::GetCursorPos());
		m_SceneViewportPanelSize = *((glm::vec2*) & ImGui::GetContentRegionAvail());

		auto sceneViewportColorAttachment = m_SceneViewportFramebuffer->GetColorAttachmentRenderID(0);
		ImGui::Image((void*)sceneViewportColorAttachment, { m_SceneViewportSize.x, m_SceneViewportSize.y }, { 0, 1 }, { 1, 0 });

		auto& hierarcy = PanelManager.GetPanel("SceneHierarcy");

		if (hierarcy->HasAnEntitySelected()) {
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_SceneViewportPanelPosition.x, m_SceneViewportPanelPosition.y, m_SceneViewportPanelSize.x, m_SceneViewportPanelSize.y);
			auto& transformComp = hierarcy->CurrentlySelectedEntity().GetComponent<TransformComponent>();

			ImGuizmo::Manipulate(glm::value_ptr(m_EditorCamera->GetViewMatrix()), glm::value_ptr(m_EditorCamera->GetProjectionMatrix()), (ImGuizmo::OPERATION) m_Gizmo, ImGuizmo::LOCAL, glm::value_ptr(transformComp.Transform));

		}


		ImGui::End();
		ImGui::PopStyleVar();





		// game viewport panel
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Game Viewport", 0, ImGuiWindowFlags_NoCollapse);

		const char* items[] = { "Any Aspect", "16x9", "4x3" };
		static const char* current_item = items[0];

		if (ImGui::BeginCombo("Aspect Ratio", current_item)) // The second parameter is the label previewed before opening the combo.
		{
			for (auto & item : items) {
				bool is_selected = (current_item == item); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(item, is_selected))
					current_item = item;

				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
		ImGui::Separator();


		int padding = ImGui::GetCursorPosY();

		m_IsGameViewportHovered = ImGui::IsWindowHovered();
		m_IsGameViewportFocused = ImGui::IsWindowFocused();

		if (m_IsGameViewportHovered && (Input::IsMouseButtonDown(MouseButton::Right) || Input::IsMouseButtonDown(MouseButton::Middle))) {
			m_IsGameViewportHovered = true;
			ImGui::SetWindowFocus();
		}



		m_GameViewportPanelPosition = *((glm::vec2*) & ImGui::GetWindowPos());
		m_GameViewportPanelPosition += *((glm::vec2*) & ImGui::GetCursorPos());
		if (current_item == items[1]) {
			m_GameViewportPanelSize = *((glm::vec2*) & ImGui::GetContentRegionAvail());
			m_GameViewportPanelSize.y = m_GameViewportPanelSize.x / 16.0f * 9.0f;
		}
		else if (current_item == items[2]) {
			m_GameViewportPanelSize = *((glm::vec2*) & ImGui::GetContentRegionAvail());
			m_GameViewportPanelSize.y = m_GameViewportPanelSize.x / 4.0f * 3.0f;
		}
		else {
			m_GameViewportPanelSize = *((glm::vec2*) & ImGui::GetContentRegionAvail());
		}

		glm::vec2 cursorPos = { ImGui::GetWindowSize().x - m_GameViewportSize.x, ImGui::GetWindowSize().y - m_GameViewportSize.y + padding };
		cursorPos.x *= 0.5f;
		cursorPos.y *= 0.5f;
		ImGui::SetCursorPos({ cursorPos.x, cursorPos.y });

		uint64_t gameViewportColorAttachment = m_GameViewportFramebuffer->GetColorAttachmentRenderID(0);
		ImGui::Image((void*)gameViewportColorAttachment, { m_GameViewportSize.x, m_GameViewportSize.y }, { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();


		// TODO: This should be moved into it's own class.
		// This is referring to the settings of the physics, such as gravity for all objects. 
		if (m_OpenPhysicsPanel) {

			ImGui::Begin("Physics Properties", &m_OpenPhysicsPanel);

			auto PhysicsWorldComp = m_EditorScene->GetAllEntitiesWith<PhysicsWorldComponent>();
			for (auto s : PhysicsWorldComp) {
				Entity e = { s, m_EditorScene.get() };
				
				auto& physicsComp = e.GetComponent<PhysicsWorldComponent>();
				if(ImGui::InputFloat2("Gravity", glm::value_ptr(physicsComp.Gravity), "%.2f")) {
					physicsComp.World->SetGravity({ physicsComp.Gravity.x, physicsComp.Gravity.y});
				}
				ImGui::InputFloat("Fixed Timestep", &physicsComp.FixedTimeStep, 0.0f, 0.0f, "%.2f");
				ImGui::InputInt("Velocity Iterations", &physicsComp.VelocityIterations);
				ImGui::InputInt("Position Iterations", &physicsComp.PositionIterations);


				// TODO: physics collision layer matrix

				

			}


			ImGui::End();

		}


		if (m_OpenSceneManagerPanel) {
			ImGui::Begin("Scene Manager", &m_OpenSceneManagerPanel);
			int iterator = 0;
			ImGui::Columns(2);
			ImGui::PushItemWidth(-1);
			ImGui::Text("Scenes Filepath");
			ImGui::NextColumn();
			ImGui::Text("Scene Index");
			ImGui::NextColumn();
			for (auto& scene : SceneManager::GetAllProjectScenesFilepath()) {
				ImGui::Separator();
				ImGui::Text(scene.c_str());
				ImGui::NextColumn();
				ImGui::Text(std::to_string(iterator).c_str());

				ImGui::NextColumn();
				iterator++;
			}

			ImGui::Columns(1);
			ImGui::Separator();
			if (ImGui::Button("Add Current Scene To The Managers Database")) {
				SceneManager::RegisterScene(SceneManager::GetActiveSceneFilepath());
			}
			ImGui::PopItemWidth();
			ImGui::End();
		}


// for creating and loading projects

// 		if (m_OpeningModal) {
// 			ImGui::OpenPopup("Load Project");
// 			m_OpeningModal = false;
// 		}
// 
// 		ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, {0.5f, 0.5f});
// 		if (ImGui::BeginPopupModal("Load Project", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
// 			auto& app = Application::GetApp();
// 			
// 			ImGui::SetWindowSize({ 300 * 16.0f / 9.0f, 300 });
// 			ImVec2 windowSize = ImGui::GetWindowSize();
// 
// 			ImGui::SetWindowPos({ app.GetWindow().GetXPos() + Application::GetApp().GetWindow().GetWidth() / 2.0f - windowSize.x / 2.0f,
// 				app.GetWindow().GetYPos() + Application::GetApp().GetWindow().GetHeight() / 2.0f  - windowSize.y / 2.0f });
// 			
// 
// 			if (ImGui::Button("Close"))
// 				ImGui::CloseCurrentPopup();
// 
// 			ImGui::EndPopup();
// 		}
// 		ImGui::PopStyleVar();



#endif


	}

}