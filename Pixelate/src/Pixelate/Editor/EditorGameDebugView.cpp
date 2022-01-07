#include "PXpch.h"
#include "EditorGameDebugView.h"


#include "Pixelate/Asset/AssetManager.h"
#include "Pixelate/Scene/SceneManager.h"

#include "Pixelate/Audio/Audio.h"
#include "Pixelate/Core/Application.h"

#include <Imgui/imgui.h>
#include <imgui_widgets.cpp>
#include <Imgui/imgui_internal.h>

#include "Pixelate/Imgui/ImGuizmo.h"


#include <glm/gtc/type_ptr.hpp>

#include "Pixelate/Events/KeyEvents.h"


namespace Pixelate
{

	EditorGameDebugView::EditorGameDebugView()
	{
		ViewportPanelProps props;
		props.Position = &m_ViewportPanelPosition;
		props.Size = &m_ViewportPanelSize;

		m_EditorCamera = CreateRef<EditorCamera>(16.0f / 9.0f, props);
		m_EditorCamera->SetOrthographicSize(5.0f);


		m_Gizmo = ImGuizmo::TRANSLATE;


	}

	void EditorGameDebugView::OnUpdate(float ts)
	{
		if (IsPanelFocused()) {
			m_EditorCamera->OnUpdate(ts);
		}

		if (m_ViewportSize != m_ViewportPanelSize) {
			m_Renderer->GetGeometryPass()->FrameBufferTarget->Resize(m_ViewportPanelSize.x, m_ViewportPanelSize.y);
			m_ViewportSize = m_ViewportPanelSize;
			m_EditorCamera->Resize(m_ViewportPanelSize.x / m_ViewportPanelSize.y);
		}

	}

	void EditorGameDebugView::OnImguiRender()
	{

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("Scene Viewport", 0, ImGuiWindowFlags_NoCollapse);


		static glm::vec4 sceneColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		ImGui::PushItemWidth(250);
		ImGui::ColorEdit4("Scene color", glm::value_ptr(sceneColor));
		float ColorRectYSize = ImGui::GetItemRectSize().y;
		ImGui::PopItemWidth();


		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y + ColorRectYSize;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;

		ImRect dragAreaRect = { vMin , vMax };
		auto sceneViewportID = ImGui::GetID("Scene Viewport");


		if (ImGui::BeginDragDropTargetCustom(dragAreaRect, sceneViewportID)) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AssetPayload"))
			{


				AssetMetadata& metadata = *(AssetMetadata*)payload->Data;
				std::filesystem::path filepath = AssetManager::GetFilePath(metadata);



				PX_CORE_MSG("Scene viewport recieved '%s'!\n", filepath.string().c_str());

				switch (metadata.Type)
				{
				case AssetType::Scene:
				{
					m_SceneContext = SceneManager::LoadScene(filepath.string());
					//PanelManager.SetSceneContext(m_EditorScene);
					break;
				}
				case AssetType::Texture:
				{
					Entity entity = m_SceneContext->CreateEntity(filepath.stem().string());
					entity.GetComponent<TransformComponent>().SetPosition(m_EditorCamera->GetPos());
					SpriteRendererComponent& comp = entity.AddComponent<SpriteRendererComponent>();
					comp.Texture = AssetManager::GetAsset<Texture>(metadata.Handle);
					comp.Rect = { {0, 0}, {comp.Texture->GetWidth(), comp.Texture->GetHeight()} };
					break;
				}
				case AssetType::Audio:
				{
					Entity entity = m_SceneContext->CreateEntity(filepath.stem().string());
					entity.GetComponent<TransformComponent>().SetPosition(m_EditorCamera->GetPos());
					AudioSourceComponent& comp = entity.AddComponent<AudioSourceComponent>();

					Ref<AudioBuffer> buffer = AssetManager::GetAsset<AudioBuffer>(metadata.Handle);
					comp.Source = Audio::CreateAudioSource(buffer);

					comp.FilePath = metadata.Filepath.string();

					//comp.Source->GetMixerGroup()->SourcesAttached[(UUID)entity.GetHandle()] = comp;
					Audio::AttachSourceToMixerGroup(comp.Source);
					//Audio::DetachSourceFromMixerGroup(comp.Source);

					break;
				}

				}

			}
			ImGui::EndDragDropTarget();
		}



		RenderCommand::SetClearColor(sceneColor.r, sceneColor.g, sceneColor.b, sceneColor.a);
		ImGui::Separator();

		m_IsViewportHovered = ImGui::IsWindowHovered();
		m_IsViewportFocused = ImGui::IsWindowFocused();

		if (m_IsViewportHovered && (Input::IsMouseButtonDown(MouseButton::Right) || Input::IsMouseButtonDown(MouseButton::Middle))) {
			m_IsViewportFocused = true;
			ImGui::SetWindowFocus();
		}


		Application::GetApp().GetImguiLayer().ShouldBlockEvents(false);


		{
			glm::vec2 WindowPos = { ImGui::GetWindowPos().x,  ImGui::GetWindowPos().y };
			glm::vec2 CursorPos = { ImGui::GetCursorPos().x, ImGui::GetCursorPos().y };
			glm::vec2 ContentRegionAvail = { ImGui::GetContentRegionAvail().x,  ImGui::GetContentRegionAvail().y };


			m_ViewportPanelPosition = WindowPos;
			m_ViewportPanelPosition += CursorPos;
			m_ViewportPanelSize = ContentRegionAvail;

		}

		auto sceneViewportColorAttachment = m_Renderer->GetGeometryPass()->FrameBufferTarget->GetColorAttachmentRenderID(0);
		ImGui::Image((void*)sceneViewportColorAttachment, { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });


		if (HasAnEntitySelected()) {
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportPanelPosition.x, m_ViewportPanelPosition.y, m_ViewportPanelSize.x, m_ViewportPanelSize.y);
			auto& transformComp =CurrentlySelectedEntity().GetComponent<TransformComponent>();

			ImGuizmo::Manipulate(glm::value_ptr(m_EditorCamera->GetViewMatrix()), glm::value_ptr(m_EditorCamera->GetProjectionMatrix()), (ImGuizmo::OPERATION) m_Gizmo, ImGuizmo::LOCAL, glm::value_ptr(transformComp.Transform));

		}


		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorGameDebugView::OnEvent(Event& e)
	{
		if (m_IsViewportFocused)
			m_EditorCamera->OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&EditorGameDebugView::OnKeyPressedEvent, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&EditorGameDebugView::OnMouseButtonPressed, this, std::placeholders::_1));

	}

	bool EditorGameDebugView::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		switch ((KeyCode)e.GetKeyCode())
		{
			case KeyCode::Q: {
				if (!ImGuizmo::IsUsing()) {
					m_Gizmo = ImGuizmo::TRANSLATE;
					break;
				}
			}

			case KeyCode::W: {
				if (!ImGuizmo::IsUsing()) {
					m_Gizmo = ImGuizmo::ROTATE;
					break;
				}
			}

			case KeyCode::E: {
				if (!ImGuizmo::IsUsing()) {
					m_Gizmo = ImGuizmo::SCALE;
					break;
				}
			}

		}


		return false;
	}

	bool EditorGameDebugView::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{


		if (e.GetButton() == (int)MouseButton::Left && e.GetRepeatCount() == 0 && m_IsViewportHovered) {

			auto [mx, my] = ImGui::GetMousePos();
			mx -= m_ViewportPanelPosition.x;
			my -= m_ViewportPanelPosition.y;
			my = m_ViewportPanelSize.y - my;
			m_Renderer->GetGeometryPass()->FrameBufferTarget->Bind();
			int px = m_Renderer->GetGeometryPass()->FrameBufferTarget->ReadPixel(1, (int)mx, (int)my);
			PX_CORE_MSG("Entity handle: %d, (%f, %f, (w) %f (h)%f)\n", px, mx, my, m_ViewportPanelSize.x, m_ViewportPanelSize.y);
			if((mx >= 0 && mx < m_ViewportPanelSize.x)&&(my >= 0 && my< m_ViewportPanelSize.y))
			{
				if (px != -1 && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver()) {
					Entity e = { (entt::entity)px, m_SceneContext.get() };
					EditorPanelManager::Get().SetSelectedEntity(e);
				}
				else if (!ImGuizmo::IsUsing() && !ImGuizmo::IsOver()) {
					EditorPanelManager::Get().SetSelectedEntity({});
				}
			}
			m_Renderer->GetGeometryPass()->FrameBufferTarget->Unbind();


		}

		return false;
	}

}