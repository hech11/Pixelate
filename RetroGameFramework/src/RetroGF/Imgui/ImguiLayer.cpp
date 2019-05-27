#include "RGFpch.h"
#ifndef RGF_DISTRIBUTE

#include "ImguiLayer.h"


#include <IMGUI/imgui.h>
#include <IMGUI/examples/imgui_impl_glfw.h>
#include <IMGUI/examples/imgui_impl_opengl3.h>

#include <GLFW/include/GLFW/glfw3.h>

#include "RetroGF/Application.h"


#include <iomanip>

namespace RGF {


	 
	void ImguiLayer::Init()  {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::GetApp();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());


		ImGui_ImplOpenGL3_Init("#version 400");
		ImGui_ImplGlfw_InitForOpenGL(window, true);
	}
	void ImguiLayer::ShutDown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImguiLayer::Start() {
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
	void ImguiLayer::End() {
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


	void ImguiLayer::OnImguiRender() {
/*
		static bool s_MiscWindowOpen = false;

		ImGui::Begin("MISC", &s_MiscWindowOpen, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar);

		auto& Window = Application::GetApp().GetWindow();
		ImGui::SetWindowPos({Window.GetXPos() + (Window.GetWidth() / 2.0f) - (ImGui::GetWindowSize().x / 2.0f),  Window.GetYPos() + 25.0f });
		std::stringstream ss;

		ss << "FPS: " << std::fixed << std::setprecision(1) << ImGui::GetIO().Framerate
			<< "(" << (1.0f / ImGui::GetIO().Framerate*1000.0f) << "ms)"
			<< "\t|\tPos: " << Window.GetXPos() << "x" << Window.GetYPos() << "\t|\tDim: " << Window.GetWidth() << "x"
			<< Window.GetHeight();

		ImGui::SameLine((ImGui::GetWindowWidth() / 2.0f) - ImGui::CalcTextSize(ss.str().c_str()).x / 2.0f);
		ImGui::Text(ss.str().c_str());


		ImGui::End();
		*/

	}


}

#endif