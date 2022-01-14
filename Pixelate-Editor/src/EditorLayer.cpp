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

#include <Pixelate/Asset/AssetManager.h>
#include "Pixelate/Rendering/SceneRenderer.h"
#include "Pixelate/Rendering/Material.h"
#include "Pixelate/Rendering/MaterialManager.h"

namespace Pixelate {


	static bool GoTo2ndClip = false;
	static bool GoTo1stClip = false;
	void EditorLayer::Init() {

		m_GameSceneRenderer = CreateRef<SceneRenderer>();
		m_EditorSceneRenderer = CreateRef<SceneRenderer>();

		m_EditorSceneRenderer->SetOptions(SceneRendererOptions::RenderAll);


		m_EditorScene = SceneManager::GenerateDefaultScene();

		auto& PanelManager = EditorPanelManager::Get();
		PanelManager.RegisterPanel("SceneHierarcy", m_SceneHierarcyPanel = CreateRef<EditorSceneHierarchyPanel>(m_EditorScene));
		PanelManager.RegisterPanel("TextureInspector" ,m_TextureInspector = CreateRef<EditorTextureInspector>(), false);
		PanelManager.RegisterPanel("ConsoleLog", m_ConsoleLog = CreateRef<EditorConsoleLogger>(true));
		PanelManager.RegisterPanel("ContentBrowser", m_ContentBrowser = CreateRef<EditorContentBrowser>());
		PanelManager.RegisterPanel("AudioPanel", m_AudioPanel = CreateRef<EditorAudioPanel>());
		PanelManager.RegisterPanel("AudioMixerPanel", m_AudioMixerPanel = CreateRef<EditorAudioMixerPanel>());
		PanelManager.RegisterPanel("PhysicsPropertiesPanel", m_PhysicsPanel = CreateRef<EditorPhysicsPropertiesPanel>());
		PanelManager.RegisterPanel("RendererPanel", m_RendererPanel = CreateRef<EditorRendererPanel>());
		PanelManager.RegisterPanel("GameView", m_GameViewPanel = CreateRef<EditorGameViewPanel>());
		PanelManager.RegisterPanel("GameDebugView", m_GameDebugViewPanel = CreateRef<EditorGameDebugView>());
		PanelManager.RegisterPanel("MaterialEditor", m_MaterialEditorPanel = CreateRef<EditorMaterialEditorPanel>());


		m_GameViewPanel->SetSceneRenderer(m_GameSceneRenderer);
		m_GameDebugViewPanel->SetSceneRenderer(m_EditorSceneRenderer);

		m_AudioMixerPanel->SetOpenPanel(false);
		m_PhysicsPanel->SetOpenPanel(false);


		// testing asset handles directly
		assetHandleTestEntity = m_EditorScene->CreateEntity("Texture loaded via asset handle");
		assetHandleTestEntity.AddComponent<SpriteRendererComponent>();
		assetHandleTestEntity.GetComponent<SpriteRendererComponent>().Texture = AssetManager::GetAsset<Texture>((AssetHandle)18069192197029962527);
		assetHandleTestEntity.GetComponent<SpriteRendererComponent>().Rect = Rect({ 0, 0 }, {128, 128});


		FileSystem::StartWatching(); // Should move this when projects are introduced.

		m_AudioMixerPanel->SetMixerContext(Audio::GetGlobalMixer());

		PanelManager.SetSceneContext(m_EditorScene);
		Application::GetApp().GetWindow().SetVsync(true);


	}


	void EditorLayer::ShutDown() {
		FileSystem::StopWatching(); // Should move this when projects are introduced.
	}

	void EditorLayer::OnUpdate(float dt) {
		auto& PanelManager = EditorPanelManager::Get();


 		PanelManager.OnUpdate(dt);

 		m_EditorScene->OnUpdate(m_EditorSceneRenderer, m_GameDebugViewPanel->GetEditorCamera());
		m_EditorScene->OnUpdate(m_GameSceneRenderer);


		if (m_SceneState == SceneState::Play) {
			m_EditorScene->OnRuntimeUpdate(dt);
			Audio::Update();
			bool updateHierarcy = false;
			SceneManager::LoadAnyQueuedScenes(m_EditorScene, &updateHierarcy);
			if (updateHierarcy)
				m_SceneHierarcyPanel->SetSceneContext(m_EditorScene);

		}

		AssetManager::ApplyAssetChanges();
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

		

		}

		return false;
	}


	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{

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

		if (m_ShowStartupModal) {
			ImGui::OpenPopup("Welcome");
			m_ShowStartupModal = false;
		}

		


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
				if (ImGui::MenuItem("Reload CSharp Assembly", "Doesn't work yet")) {
					//ScriptingMaster::ReloadAssembly();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings")) {
				if (ImGui::MenuItem("Scenes", "")) {
					m_OpenSceneManagerPanel = true;
				}
				if (ImGui::MenuItem("Rendering", "")) {
					m_RendererPanel->SetOpenPanel(true);
				}
				if (ImGui::MenuItem("Physics", "")) {
					m_PhysicsPanel->SetOpenPanel(true);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window")) {

				if (ImGui::MenuItem("Audio", "")) {
					m_AudioPanel->SetOpenPanel(true);
				}
				if (ImGui::MenuItem("Audio Mixer", "")) {
					m_AudioMixerPanel->SetOpenPanel(true);
				}
				if (ImGui::MenuItem("Console Logger", "")) {
					m_ConsoleLog->SetOpenPanel(true);
				}
				if (ImGui::MenuItem("Content Browser", "")) {
					m_ContentBrowser->SetOpenPanel(true);
				}
				if (ImGui::MenuItem("Game View", "")) {
					m_GameViewPanel->SetOpenPanel(true);
				}
				if (ImGui::MenuItem("Scene View", "")) {
					m_GameViewPanel->SetOpenPanel(true);
				}
				if (ImGui::MenuItem("Material Editor", "")) {
					m_MaterialEditorPanel->SetOpenPanel(true);
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}





		ImGui::End();
/////////////////////////////////////////////////////////////////////////////////////



//  		static bool showDemoWindow = false;
//  		ImGui::ShowDemoWindow(&showDemoWindow);

	


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


		if (ImGui::BeginPopupModal("Welcome", NULL, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize)) {
			auto& appWindow = Application::GetApp().GetWindow();

			ImVec2 windowSize = ImGui::GetWindowSize();
			float x = appWindow.GetXPos() + (appWindow.GetWidth() / 2.0f) + windowSize.x / 2.0f;
			float y = appWindow.GetYPos() + (appWindow.GetHeight() / 2.0f) + windowSize.y / 2.0f;
			ImGui::SetWindowPos({x, y});

			ImGui::Text("Hello! Thank you for downloading Pixelate!");
			ImGui::Text("Pixelate is currently a WIP so features here may unfinished or broken.");
			ImGui::Text("With that in mind please be aware that this program may crash!");
			ImGui::Separator();
			if (ImGui::Button("Close", { 500, 50 })) { ImGui::CloseCurrentPopup(); }
			ImGui::SetItemDefaultFocus();
			ImGui::EndPopup();
		}


#endif


	}

}