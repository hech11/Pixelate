
#include "RGFpch.h"
#include "EditorViewportPanel.h"

#include "imgui.h"
#include "RetroGF/Core/Application.h"

#include "RetroGF/Physics/Physics.h"

#include "Misc/ColorStyleEditor.h"

#include "RetroGF/Core/Input.h"
#include "RetroGF/Core/MouseButtonCodes.h"
#include "glm/gtc/type_ptr.inl"

#include "RetroGF/Rendering/RenderCommand.h"
#include "RetroGF/Imgui/ImGuizmo.h"
#include "RetroGF/Scene/Components.h"
#include "../Core/KeyCodes.h"
#include "glm/ext/quaternion_float.hpp"
#include "glm/gtx/matrix_decompose.hpp"

namespace RGF {


	EditorViewportPanel::EditorViewportPanel(const Ref<EditorSceneHierarchyPanel>& panel) 
		: m_EditorHierarchy(panel)
	{
		FrameBufferSpecs ViewportSpecs;
		ViewportSpecs.Width = 960;
		ViewportSpecs.Height = 540;


		m_ViewportFramebuffer = FrameBuffer::Create(ViewportSpecs);
		m_EditorCamera = CreateScoped<EditorCamera>(16.0f / 9.0f);



		Physics::GetDebug().SetCamera(&m_EditorCamera->GetCamera());
		//Application::GetApp().PushOverlay(new ColorStyleEditor);
		m_Gizmo = ImGuizmo::TRANSLATE;

	}


	void EditorViewportPanel::OnEvent(Event& e)
	{
		m_EditorCamera->OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(RGF_BIND_EVENT_FNC(EditorViewportPanel::IsKeyDownEvent));
	}

	void EditorViewportPanel::OnUpdate(float ts) {

		if (m_ViewportSize != m_ViewportPanelSize) {
			m_ViewportFramebuffer->Resize(m_ViewportPanelSize.x, m_ViewportPanelSize.y);
			m_ViewportSize = m_ViewportPanelSize;
			m_EditorCamera->Resize(m_ViewportPanelSize.x, m_ViewportPanelSize.y);
		}


		if(m_IsViewportHovered)
			m_EditorCamera->OnUpdate(ts, {m_ViewportPanelPosition, m_ViewportPanelSize});

	}

	void EditorViewportPanel::OnImguiRender() {
		using namespace RGF;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Viewport", 0, ImGuiWindowFlags_NoCollapse);
// 		static glm::vec4 sceneColor = {0.2f, 0.2f, 0.2f, 1.0f};
// 		ImGui::PushItemWidth(250);
// 		ImGui::ColorEdit4("Scene color", glm::value_ptr(sceneColor));
// 		ImGui::PopItemWidth();
// 		RenderCommand::SetClearColor(sceneColor.r, sceneColor.g, sceneColor.b, sceneColor.a);
		ImGui::Separator();
		auto colorAttachment = m_ViewportFramebuffer->GetColorAttachment();

		m_IsViewportHovered = ImGui::IsWindowHovered();
		m_IsViewportFocused = ImGui::IsWindowFocused();

		if (m_IsViewportHovered && (Input::IsMouseButtonDown(RGF_MOUSE_BUTTON_RIGHT) || Input::IsMouseButtonDown(RGF_MOUSE_BUTTON_MIDDLE))) {
			m_IsViewportFocused = true;
			ImGui::SetWindowFocus();
		}


		Application::GetApp().GetImguiLayer().ShouldBlockEvents(!m_IsViewportHovered);

		m_ViewportPanelPosition = *((glm::vec2*) & ImGui::GetWindowPos());
		m_ViewportPanelPosition += *((glm::vec2*) & ImGui::GetCursorPos());
		m_ViewportPanelSize = *((glm::vec2*) & ImGui::GetContentRegionAvail());
		ImGui::Image((void*)colorAttachment, { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });

		if (m_EditorHierarchy->HasAnEntitySelected()) {
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(m_ViewportPanelPosition.x, m_ViewportPanelPosition.y, m_ViewportPanelSize.x, m_ViewportPanelSize.y);
			auto& transformComp = m_EditorHierarchy->CurrentlySelectedEntity().GetComponent<TransformComponent>();
			
			ImGuizmo::Manipulate(glm::value_ptr(m_EditorCamera->GetCamera().GetViewMatrix()), glm::value_ptr(m_EditorCamera->GetCamera().GetProjectionMatrix()), (ImGuizmo::OPERATION) m_Gizmo, ImGuizmo::LOCAL, glm::value_ptr(transformComp.Transform));

		}

		ImGui::End();
		ImGui::PopStyleVar();


	}

	void EditorViewportPanel::DrawToViewport() {
		m_ViewportFramebuffer->Bind();
	}

	void EditorViewportPanel::FinishDrawing() {
		m_ViewportFramebuffer->Unbind();
	}

	bool EditorViewportPanel::IsKeyDownEvent(KeyPressedEvent& e)
	{

		if (e.GetKeyCode() == RGF_KEY_Q) {
			m_Gizmo = ImGuizmo::TRANSLATE;
		}
		if (e.GetKeyCode() == RGF_KEY_W) {
			m_Gizmo = ImGuizmo::ROTATE;

		}
		if (e.GetKeyCode() == RGF_KEY_E) {
			m_Gizmo = ImGuizmo::SCALE;

		}
		return false;
	}

}