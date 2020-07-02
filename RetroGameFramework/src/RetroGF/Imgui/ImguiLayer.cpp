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

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_::ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::GetApp();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());


		ImGui_ImplOpenGL3_Init("#version 400");
		ImGui_ImplGlfw_InitForOpenGL(window, false);


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
	}
	void ImguiLayer::End() {

		RGF_PROFILE_FUNCTION();
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
	
		
	}



}

#endif