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
#include "RetroGF/Rendering/Renderer.h"


#include <iomanip>


namespace RGF {

	void ImguiEngineEditor::Init()  {
	}
	void ImguiEngineEditor::ShutDown() {
	}

	void ImguiEngineEditor::Start() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_WindowBg, IM_COL32(200, 0, 150, 150));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Border, IM_COL32(200, 0, 150, 200));

		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TitleBg, IM_COL32(200, 0, 150, 100));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TitleBgActive, IM_COL32(200, 0, 150, 255));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TitleBgCollapsed, IM_COL32(200, 0, 150, 50));

		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ResizeGrip, IM_COL32(200, 0, 150, 150));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ResizeGripActive, IM_COL32(200, 0, 150, 255));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ResizeGripHovered, IM_COL32(200, 0, 150, 100));


		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_SliderGrab, IM_COL32(200, 0, 150, 100));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_SliderGrabActive, IM_COL32(200, 0, 150, 150));


		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Header, IM_COL32(55, 0, 45, 255));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_HeaderActive, IM_COL32(55, 0, 45, 255));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_HeaderHovered, IM_COL32(55, 0, 45, 255));


		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, IM_COL32(200, 0, 150, 150));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBgActive, IM_COL32(200, 0, 150, 200));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBgHovered, IM_COL32(200, 0, 150, 255));

		
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, IM_COL32(200, 0, 150, 150));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBgActive, IM_COL32(200, 0, 150, 200));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBgHovered, IM_COL32(200, 0, 150, 255));


	}
	void ImguiEngineEditor::End() {
		ImGui::PopStyleColor(19);

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		Application& app = Application::GetApp();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(context);

		}


	}


	void ImguiEngineEditor::OnImguiRender() {

		ImGui::Begin("Game viewport");
		ImGui::GetWindowDrawList()->AddImage((void*)1, { -0.5f, -0.5f }, { -0.5f, -0.5f });
		ImGui::End();
	}



}

#endif