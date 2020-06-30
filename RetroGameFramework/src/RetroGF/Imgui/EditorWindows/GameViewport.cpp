#include "RGFpch.h"

#ifdef RGF_USE_IMGUI


#include "GameViewport.h"

#include <IMGUI/imgui.h>
#include <IMGUI/examples/imgui_impl_glfw.h>
#include <IMGUI/examples/imgui_impl_opengl3.h>

#include <GLAD/include/glad.h>
#include <GLFW/include/GLFW/glfw3.h>


#include "RetroGF/Core/Application.h"
#include "RetroGF/Rendering/Renderer2D.h"
#include "RetroGF/Rendering/RenderCommand.h"
#include "RetroGF/Core/Input.h"
#include "RetroGF/Core/MouseButtonCodes.h"

#include <iomanip>


namespace RGF {

	void GameViewport::Init()  {
		GameViewCameraController = RGF::CreateScoped<OrthographicCameraController>(960/540, true);

	}
	void GameViewport::ShutDown() {
		delete ViewportFBO;
	}

	void GameViewport::Start() {
	

	}
	void GameViewport::End() {
	

	}

	#if 0

bool GameViewport::ZoomCamera(MouseScrolledEvent& e) {

		if (e.GetYScroll() > 0)
			m_ScaleSensitivity -= 0.01f * (m_ScaleSensitivity * 4);
		else
			m_ScaleSensitivity += 0.01f * (m_ScaleSensitivity * 4);


		GameViewCamera->SetScale({ m_ScaleSensitivity, m_ScaleSensitivity, 1.0f });
		return true;
	}


	bool GameViewport::MoveCamera(MouseMovedEvent& e) {

		
		const auto& window = static_cast<GLFWwindow*>(Application::GetApp().GetWindow().GetNativeWindow());
		// TODO: Implement a raw mouse movement into the api. This is temp.
		if (Input::IsMouseButtonDown(RGF_MOUSE_BUTTON_3)) {

			static float LastMouseX = 0.0f, LastMouseY = 0.0f;
			static bool firstMouseFocus = true;

			if (firstMouseFocus) {
				LastMouseX = e.GetMousePos().first;
				LastMouseY = e.GetMousePos().second;
				firstMouseFocus = false;
			}

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


			float xOffset = e.GetMousePos().first - LastMouseX;
			float yOffset = LastMouseY - e.GetMousePos().second;
			LastMouseX = e.GetMousePos().first;
			LastMouseY = e.GetMousePos().second;

			xOffset *= m_ScaleSensitivity;
			yOffset *= m_ScaleSensitivity;

			GameViewCamera->Move({ -xOffset, -yOffset, 0.0f });


		} else {
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}



		return true;
	}
#endif


	void GameViewport::OnEvent(Event& e) {
		GameViewCameraController->OnEvent(e);
	}

	void GameViewport::OnUpdate(float dt) {
//		RenderCommand::Clear();
		GameViewCameraController->OnUpdate(dt);
	}


	void GameViewport::OnImguiRender() {
		// The game viewport.
		auto& Window = Application::GetApp().GetWindow();


		ImGui::Begin("Game viewport", (bool*)0, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);

		const auto gameviewportWindowPos = ImGui::GetWindowPos();
		const auto gameviewportWindowScale = ImGui::GetWindowSize();

		m_GameviewWindowSize = { gameviewportWindowPos.x, gameviewportWindowPos.y };
		m_GameviewWindowPos = { gameviewportWindowScale.x, gameviewportWindowScale.y };

		const ImVec2& windowscale = ImGui::GetWindowSize();
		ImGui::End();


		// Properties window.
		ImGui::Begin("Props", (bool*)0, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);
		const auto propsWindowScale = ImGui::GetWindowSize();

		ImGui::SetWindowPos({ gameviewportWindowPos.x + gameviewportWindowScale.x - propsWindowScale.x - 25.0f, gameviewportWindowPos.y + 50.0f });
		width = gameviewportWindowScale.x;
		height = gameviewportWindowScale.y;

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

