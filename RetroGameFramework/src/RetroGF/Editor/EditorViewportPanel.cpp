
#include "RGFpch.h"
#include "EditorViewportPanel.h"

#include "imgui.h"
#include "RetroGF/Core/Application.h"

#include "RetroGF/Physics/Physics.h"

#include "Misc/ColorStyleEditor.h"

#include "RetroGF/Core/Input.h"
#include "RetroGF/Core/MouseButtonCodes.h"

namespace RGF {


	EditorViewportPanel::EditorViewportPanel() {
		FrameBufferSpecs ViewportSpecs;
		ViewportSpecs.Width = 960;
		ViewportSpecs.Height = 540;


		m_ViewportFramebuffer = FrameBuffer::Create(ViewportSpecs);
		m_EditorCamera = CreateScoped<EditorCamera>(16.0f / 9.0f);



		Physics::GetDebug().SetCamera(&m_EditorCamera->GetCamera());
		//Application::GetApp().PushOverlay(new ColorStyleEditor);
	}


	void EditorViewportPanel::OnEvent(Event& e)
	{
		m_EditorCamera->OnEvent(e);
	}

	void EditorViewportPanel::OnUpdate(float ts) {

		if (m_ViewportSize != m_ViewportPanelSize) {
			m_ViewportFramebuffer->Resize(m_ViewportPanelSize.x, m_ViewportPanelSize.y);
			m_ViewportSize = m_ViewportPanelSize;
			m_EditorCamera->Resize(m_ViewportSize.x, m_ViewportSize.y);
		}


		if(m_IsViewportHovered)
			m_EditorCamera->OnUpdate(ts, {m_ViewportPanelPosition, m_ViewportPanelSize});

	}

	void EditorViewportPanel::OnImguiRender() {
		using namespace RGF;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

		ImGui::Begin("Viewport");
		auto colorAttachment = m_ViewportFramebuffer->GetColorAttachment();

		m_IsViewportHovered = ImGui::IsWindowHovered();
		m_IsViewportFocused = ImGui::IsWindowFocused();

		if (m_IsViewportHovered && (Input::IsMouseButtonDown(RGF_MOUSE_BUTTON_RIGHT) || Input::IsMouseButtonDown(RGF_MOUSE_BUTTON_MIDDLE))) {
			m_IsViewportFocused = true;
			ImGui::SetWindowFocus();
		}


		Application::GetApp().GetImguiLayer().ShouldBlockEvents(!m_IsViewportHovered);


		m_ViewportPanelPosition = *((glm::vec2*) & ImGui::GetWindowPos());
		m_ViewportPanelSize = *((glm::vec2*) & ImGui::GetContentRegionAvail());
		ImGui::Image((void*)colorAttachment, { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();


	}

	void EditorViewportPanel::DrawToViewport() {
		m_ViewportFramebuffer->Bind();
	}

	void EditorViewportPanel::FinishDrawing() {
		m_ViewportFramebuffer->Unbind();
	}

}