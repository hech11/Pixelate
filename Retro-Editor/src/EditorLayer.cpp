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

#include <RetroGF/Rendering/RendererAPI.h>
#include <RetroGF/Core/MouseButtonCodes.h>
#include "glm/ext/quaternion_common.hpp"
#include "RetroGF/Imgui/ImGuizmo.h"
#include "glm/gtx/matrix_decompose.hpp"


namespace RGF {

	void EditorLayer::Init() {

		Application::GetApp().GetWindow().SetTitle("Retro-Editor");
		Ref<FrameBuffer> m_ViewportFramebuffer;
		

		particleProps.VelocityVariation1 = { -2.0f, -2.0f };
		particleProps.VelocityVariation2 = { 2.0f, 2.0f };
		particleProps.LifeTime = 1.0f;
		particleProps.SizeBegin = 1.5f;
		particleProps.SizeVariation = 0.0f;
		particleProps.SizeEnd = 0.0f;
		particleProps.ColorBegin = { 1.0f, 1.0f, 1.0f, 1.0f };
		particleProps.ColorEnd = { 0.0f, 0.0f, 1.0f , 0.0f };


		m_EditorScene = CreateRef<Scene>();
		m_SceneHierarcyPanel = CreateRef<EditorSceneHierarchyPanel>(m_EditorScene);

		m_ViewportPanel = CreateScoped<EditorViewportPanel>(m_SceneHierarcyPanel);


		Renderer2D::SetBoundingBox(true);
	}


	void EditorLayer::ShutDown() {}

	void EditorLayer::OnUpdate(float dt) {

		using namespace RGF;

		{
			m_ViewportPanel->OnUpdate(dt);
			// TOOD: This should be its own component soon
// 			if (m_ViewportPanel->IsHovered()) {
// 				if (Input::IsMouseButtonDown(0)) {
// 					float x = m_ViewportPanel->GetCamera()->GetMousePositionRelativeToViewportPanel().x;
// 					float y = m_ViewportPanel->GetCamera()->GetMousePositionRelativeToViewportPanel().y;
// 
// 
// 
// 					float width = m_ViewportPanel->GetViewportSize().x;
// 					float height = m_ViewportPanel->GetViewportSize().y;
// 
// 					auto bounds = m_ViewportPanel->GetCamera()->GetBounds();
// 					auto pos = m_ViewportPanel->GetCamera()->GetCamera().GetPos();
// 					x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
// 					y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
// 					particleProps.Position = { x + pos.x, y + pos.y };
// 
// 
// 					for (unsigned int i = 0; i < particleSystem.InitData.SpawnRate; i++) {
// 						particleSystem.Emit(particleProps);
// 					}
// 
// 				}
// 			}

		

			particleSystem.OnUpdate(dt);
			Audio::Update();


		}

		{

			m_ViewportPanel->DrawToViewport();
			m_EditorScene->OnUpdate(dt, *m_ViewportPanel->GetCamera().get());

			// temp
			m_SceneHierarcyPanel->OnUpdate(dt, *m_ViewportPanel->GetCamera().get());

// 			particleSystem.OnRender();

			Physics::DrawDebugObjects();

			m_ViewportPanel->FinishDrawing();

		}
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e) {

		return false;
	}


	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == RGF_MOUSE_BUTTON_1 && e.GetRepeatCount() == 0 && m_ViewportPanel->IsHovered()) {

			auto allSpriteEntities = m_EditorScene->GetAllEntitiesWith<SpriteRendererComponent>();
			for (auto s : allSpriteEntities) {
				Entity e = { s, m_EditorScene.get() };
				auto& transform = e.GetComponent<TransformComponent>();
				auto [position, rot, scale] = transform.DecomposeTransform();

				bool intersects = m_ViewportPanel->GetCamera()->IsIntersecting(position, scale);
				if (intersects) {
					m_SceneHierarcyPanel->SetSelectedEntity({s, m_EditorScene.get()});
				} 
			}

		}

		return false;
	}

	void EditorLayer::OnEvent(Event& e) {
		m_ViewportPanel->OnEvent(e);
		m_EditorScene->OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&EditorLayer::OnKeyPressedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&EditorLayer::OnMouseButtonPressed, this, std::placeholders::_1));

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
		std::string pos = std::to_string(m_ViewportPanel->GetCamera()->GetCamera().GetPos().x ) + ", " + std::to_string(m_ViewportPanel->GetCamera()->GetCamera().GetPos().y);
		std::string rot = std::to_string(m_ViewportPanel->GetCamera()->GetCamera().GetRot());
		std::string orthoSize = std::to_string(m_ViewportPanel->GetCamera()->GetZoomLevel());

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


		m_SceneHierarcyPanel->OnImguiRender();
		m_ViewportPanel->OnImguiRender();

#endif


	}

}