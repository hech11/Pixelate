#include "PXpch.h"
#include "EditorGameViewPanel.h"

#include <imgui.h>

namespace Pixelate
{


	void EditorGameViewPanel::OnUpdate(float ts)
	{
		if (m_ViewportSize != m_ViewportPanelSize) {
			m_Renderer->GetGeometryPass()->FrameBufferTarget->Resize(m_ViewportPanelSize.x, m_ViewportPanelSize.y);
			m_ViewportSize = m_ViewportPanelSize;
		}

		m_SceneContext->SetGameViewport(m_ViewportPanelSize.x, m_ViewportPanelSize.y);

	}

	void EditorGameViewPanel::OnImguiRender()
	{

		if (!m_IsPanelOpen)
			return;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Game Viewport", &m_IsPanelOpen);

		const char* items[] = { "Any Aspect", "16x9", "4x3" };
		static const char* current_item = items[0];

		if (ImGui::BeginCombo("Aspect Ratio", current_item)) // The second parameter is the label previewed before opening the combo.
		{
			for (auto& item : items) {
				bool is_selected = (current_item == item); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(item, is_selected))
					current_item = item;

				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
		ImGui::Separator();



		int padding = ImGui::GetCursorPosY();

		m_IsViewportHovered = ImGui::IsWindowHovered();
		m_IsViewportFocused = ImGui::IsWindowFocused();

		if (m_IsViewportHovered && (Input::IsMouseButtonDown(MouseButton::Right) || Input::IsMouseButtonDown(MouseButton::Middle))) {
			m_IsViewportHovered = true;
			ImGui::SetWindowFocus();
		}

		glm::vec2 WindowPos = { ImGui::GetWindowPos().x,  ImGui::GetWindowPos().y };
		glm::vec2 CursorPos = { ImGui::GetCursorPos().x, ImGui::GetCursorPos().y };
		glm::vec2 ContentRegionAvail = { ImGui::GetContentRegionAvail().x,  ImGui::GetContentRegionAvail().y };

		m_ViewportPanelPosition = WindowPos;
		m_ViewportPanelPosition += CursorPos;
		if (current_item == items[1]) {
			m_ViewportPanelSize = ContentRegionAvail;
			m_ViewportPanelSize.y = m_ViewportPanelSize.x / 16.0f * 9.0f;
		}
		else if (current_item == items[2]) {
			m_ViewportPanelSize = ContentRegionAvail;
			m_ViewportPanelSize.y = m_ViewportPanelSize.x / 4.0f * 3.0f;
		}
		else {
			m_ViewportPanelSize = ContentRegionAvail;
		}

		glm::vec2 cursorPos = { ImGui::GetWindowSize().x - m_ViewportSize.x, ImGui::GetWindowSize().y - m_ViewportSize.y + padding };
		cursorPos.x *= 0.5f;
		cursorPos.y *= 0.5f;
		ImGui::SetCursorPos({ cursorPos.x, cursorPos.y });

		uint64_t gameViewportColorAttachment = m_Renderer->GetGeometryPass()->FrameBufferTarget->GetColorAttachmentRenderID(0);
		ImGui::Image((void*)gameViewportColorAttachment, { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();

	}

}