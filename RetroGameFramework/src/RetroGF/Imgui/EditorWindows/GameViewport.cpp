#include "RGFpch.h"

#ifndef RGF_DISTRIBUTE

#include "GameViewport.h"

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

	void GameViewport::Init()  {
		ViewportFBO = FrameBuffer::Create(960, 540);
	}
	void GameViewport::ShutDown() {
		delete ViewportFBO;
	}

	void GameViewport::Start() {
	

	}
	void GameViewport::End() {
	

	}


	void GameViewport::OnImguiRender() {
		// The game viewport.
		ImGui::Begin("Game viewport", (bool*)0, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);
		const ImVec2& windowscale = ImGui::GetWindowSize();
		ImGui::Image((void*)ViewportFBO->GetTexture(), { windowscale.x, windowscale.y - 35.0f });
		ImGui::End();

		ViewportFBO->Unbind();


		// Properties window.
		ImGui::Begin("Props", (bool*)0, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize);

		auto & Window = Application::GetApp().GetWindow();

		ImGui::Text("FPS: %.1f(%.1f)", ImGui::GetIO().Framerate, (1.0f / ImGui::GetIO().Framerate * 1000.0f));
		ImGui::Text("Window Pos: %d,%d", Window.GetXPos(), Window.GetYPos());
		ImGui::Text("Window Size: %d,%d", Window.GetWidth(), Window.GetHeight());
		ImGui::Text("---Rendering---");
		ImGui::Text("API: %s", Application::GetApp().GetWindow().GetContext()->GetContextName().c_str());
		ImGui::Text("Vendor: %s", Application::GetApp().GetWindow().GetContext()->GetVendorName().c_str());
		ImGui::Text("GPU Card: %s", Application::GetApp().GetWindow().GetContext()->GetRendererName().c_str());
		ImGui::Text("Version: %s", Application::GetApp().GetWindow().GetContext()->GetVersion().c_str());

		ImGui::End();


	}



}

#endif