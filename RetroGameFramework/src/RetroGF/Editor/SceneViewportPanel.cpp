
#include "RGFpch.h"
#include "SceneViewportPanel.h"

#include "imgui.h"
#include "RetroGF/Core/Application.h"

#include "RetroGF/Physics/Physics.h"

namespace RGF {


	SceneViewportPanel::SceneViewportPanel() {
		FrameBufferSpecs ViewportSpecs;
		ViewportSpecs.Width = 960;
		ViewportSpecs.Height = 540;


		m_ViewportFramebuffer = FrameBuffer::Create(ViewportSpecs);
		m_SceneCamera = CreateScoped<SceneViewportCamera>(16.0f / 9.0f);



		Physics::GetDebug().SetCamera(&m_SceneCamera->GetCamera());

	}


	void SceneViewportPanel::OnEvent(Event& e)
	{
		m_SceneCamera->OnEvent(e);
	}

	void SceneViewportPanel::OnUpdate(float ts) {

		if (m_ViewportSize != m_ViewportPanelSize) {
			m_ViewportFramebuffer->Resize(m_ViewportPanelSize.x, m_ViewportPanelSize.y);
			m_ViewportSize = m_ViewportPanelSize;
			m_SceneCamera->Resize(m_ViewportSize.x, m_ViewportSize.y);
		}


		if(m_IsViewportHovered)
			m_SceneCamera->OnUpdate(ts, {m_ViewportPanelPosition, m_ViewportPanelSize});

	}

	void SceneViewportPanel::OnImguiRender() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });


		ImGui::Begin("Viewport");
		auto colorAttachment = m_ViewportFramebuffer->GetColorAttachment();

		m_IsViewportHovered = ImGui::IsWindowHovered();
		m_IsViewportFocused = ImGui::IsWindowFocused();

		//RGF_CORE_WARN("Focused: %d, Hovered: %d\n", m_IsViewportFocused, m_IsViewportHovered);

		Application::GetApp().GetImguiLayer().ShouldBlockEvents(!m_IsViewportHovered || !m_IsViewportFocused);


		m_ViewportPanelPosition = *((glm::vec2*) & ImGui::GetWindowPos());
		m_ViewportPanelSize = *((glm::vec2*) & ImGui::GetContentRegionAvail());
		ImGui::Image((void*)colorAttachment, { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();


	}

	void SceneViewportPanel::DrawToViewport() {
		m_ViewportFramebuffer->Bind();
	}

	void SceneViewportPanel::FinishDrawing() {
		m_ViewportFramebuffer->Unbind();
	}

}