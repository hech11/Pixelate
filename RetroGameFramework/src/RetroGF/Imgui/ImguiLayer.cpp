#include "RGFpch.h"

#ifdef RGF_USE_IMGUI

#include "ImguiLayer.h"

#include <IMGUI/imgui.h>
#include <IMGUI/examples/imgui_impl_glfw.h>
#include <IMGUI/examples/imgui_impl_opengl3.h>

#include <GLAD/include/glad.h>
#include <GLFW/include/GLFW/glfw3.h>



#include "RetroGF/Core/Application.h"
#include "RetroGF/Rendering/Renderer2D.h"


#include <iomanip>

#include "RetroGF/Debug/Instrumentor.h"
#include "RetroGF/Imgui/ImGuizmo.h"


namespace RGF {


	ImguiLayer::ImguiLayer() : Layer("EngineEditor") {
		Application::GetApp().PushOverlay(this);
	}



	void ImguiLayer::OnEvent(Event& e) {
		if (m_BlockEvents) {
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategory::eMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategory::eKeyboard) & io.WantCaptureKeyboard;
		}
	}


	void ImguiLayer::Init()  {

		RGF_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable;

		io.ConfigWindowsMoveFromTitleBarOnly = true;

		//ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::GetApp();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());


		ImGui_ImplOpenGL3_Init("#version 400");
		ImGui_ImplGlfw_InitForOpenGL(window, true);

		auto* font = io.Fonts->AddFontFromFileTTF("../RetroGameFramework/src/RetroGF/Editor/Misc/bahnschrift.ttf", 15.0f);
		io.FontDefault = io.Fonts->Fonts.back();

		{
			ImVec4* colors = ImGui::GetStyle().Colors;
			colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 0.78f);
			colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 1.00f, 1.00f, 0.28f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.58f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.90f);
			colors[ImGuiCol_Border] = ImVec4(0.06f, 0.06f, 0.06f, 0.60f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.11f, 0.11f, 0.11f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.78f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.11f, 0.11f, 0.11f, 0.75f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.47f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.78f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
			colors[ImGuiCol_Header] = ImVec4(0.49f, 0.49f, 0.49f, 0.76f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.86f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
			colors[ImGuiCol_Separator] = ImVec4(0.49f, 0.49f, 0.49f, 0.40f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.75f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.62f, 0.62f, 0.62f, 0.95f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.32f, 0.32f, 0.32f, 0.78f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.78f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
			colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.11f, 0.11f, 0.40f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
			colors[ImGuiCol_TabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.11f, 0.11f, 0.40f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.11f, 0.11f, 0.70f);
			colors[ImGuiCol_DockingPreview] = ImVec4(0.62f, 0.62f, 0.62f, 0.30f);
			colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 0.63f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 1.00f, 1.00f, 0.63f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.49f, 0.49f, 0.49f, 0.43f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.73f);
		}

	}
	void ImguiLayer::ShutDown() {

		RGF_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImguiLayer::Start() {

		RGF_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuizmo::BeginFrame();
		ImGuizmo::SetOrthographic(true);
	}
	void ImguiLayer::End() {

		RGF_PROFILE_FUNCTION();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		Application& app = Application::GetApp();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		{
			RGF_PROFILE_SCOPE("ImguiLayer::End::RenderImgui");

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable) {
				GLFWwindow* context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(context);

			}

		}

	}


	void ImguiLayer::OnImguiRender() {
	
		
	}



}

#endif