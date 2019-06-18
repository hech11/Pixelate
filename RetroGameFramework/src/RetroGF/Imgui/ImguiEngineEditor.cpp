#include "RGFpch.h"

#ifndef RGF_DISTRIBUTE

#include "ImguiEngineEditor.h"

#include <IMGUI/imgui.h>
#include <IMGUI/examples/imgui_impl_glfw.h>
#include <IMGUI/examples/imgui_impl_opengl3.h>

#include <GLAD/include/glad.h>
#include <GLFW/include/GLFW/glfw3.h>


#include "RetroGF/Rendering/API/FrameBuffer.h"

#include "RetroGF/Application.h"
#include "RetroGF/Rendering/Renderer2D.h"


#include <iomanip>


namespace RGF {

	void ImguiEngineEditor::Init()  {
		GameView = new GameViewport;
		RenderingProps = new RendererProperties;
		EngineColEditor = new ColorStyleEditor;
	}
	void ImguiEngineEditor::ShutDown() {
	}

	void ImguiEngineEditor::Start() {
		
	}
	void ImguiEngineEditor::End() {

	}


	void ImguiEngineEditor::OnImguiRender() {

		// Docking space and the menu bar.
		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", (bool*)0, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}


		// The menu bar.
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Themes")) {
				EngineColEditor->IsOpen = !EngineColEditor->IsOpen;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Close")) {
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();

	}



}

#endif