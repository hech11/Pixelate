#include "RGFpch.h"
#include "EditorSceneHierarchyPanel.h"

#include <Imgui/imgui.h>
#include "glm/gtc/type_ptr.inl"
#include "RetroGF/Core/MouseButtonCodes.h"
#include "RetroGF/Core/Input.h"


// TODO: Fix this filepath and implement this properly using premake
#include "../../Retro-Editor/vendor/NativeFileDialog/src/include/nfd.h"

#include "RetroGF/Rendering/Renderer2D.h"
#include "RetroGF/Imgui/ImGuizmo.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "../Core/KeyCodes.h"

namespace RGF {

#define LOCK_MOUSE_IF_NEEDED() if (ImGui::IsItemActivated()) {\
	Input::SetMouseLockMode(Input::MouseLockMode::Locked);\
}\
if (ImGui::IsItemDeactivated()) {\
Input::SetMouseLockMode(Input::MouseLockMode::None);\
}

	EditorSceneHierarchyPanel::EditorSceneHierarchyPanel(const Ref<Scene>& scene)
		:m_SceneContext(scene)
	{
	}

	void EditorSceneHierarchyPanel::SetSceneContext(const Ref<Scene>& scene)
	{
		m_SceneContext = scene;
	}
	void EditorSceneHierarchyPanel::SetSelectedEntity(Entity entity) {
		m_CurrentlySelectedEntity = entity;
		m_SelectedEntity = true;
	}
	void EditorSceneHierarchyPanel::SetSelectedEntity() {
		m_CurrentlySelectedEntity = {};
		m_SelectedEntity = false;
	}


	// Not sure if this should render the aabb of the entities should prob make a scene renderer or scene manager
	// this is temp.
	void EditorSceneHierarchyPanel::OnUpdate(float ts, EditorCamera& camera)
	{
		m_Camera = &camera;
		Renderer2D::BeginScene(&camera.GetCamera());
		if (Renderer2D::ShouldDrawBoundingBox() && m_SelectedEntity) {
			auto& entity = m_CurrentlySelectedEntity;
			if (entity.HasComponent<SpriteRendererComponent>()) {
				auto& transformComp = entity.GetComponent<TransformComponent>();
				AABB boundingBox;

				auto [Position, rot, Scale] = transformComp.DecomposeTransform();

				boundingBox.Min = { -Scale.x / 2.0f + Position.x, -Scale.y / 2.0f + Position.y, 1.0f };
				boundingBox.Max = { Scale.x / 2.0f + Position.x, Scale.y / 2.0f + Position.y, 1.0f };

				Renderer2D::DrawAABB(boundingBox, { 0.0f, 1.0f, 1.0f, 1.0f });

			}

		}

		Renderer2D::EndScene();

	}

	void EditorSceneHierarchyPanel::OnEvent()
	{

	}


	


	void EditorSceneHierarchyPanel::OnImguiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		// display all entities in hierarchy
		bool deleteEntity = false;
		m_SceneContext->GetReg().each([&](auto entity) {

			Entity e(entity, m_SceneContext.get());
			std::string name = e.GetComponent<NameComponent>().Name;

			ImGuiTreeNodeFlags node_flags = ((e == m_CurrentlySelectedEntity) && m_SelectedEntity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			if (ImGui::TreeNodeEx((void*)(unsigned int)e, node_flags, name.c_str())) {
				ImGui::TreePop();
			}

			if (ImGui::IsItemClicked()) {
				SetSelectedEntity(e);
			}

			if (Input::IsKeyDown(RGF_KEY_DELETE))
				deleteEntity = true;


			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::Button("Delete"))
					deleteEntity = true;
				ImGui::EndPopup();
			}

			if (deleteEntity) {
				if (e == m_CurrentlySelectedEntity) {
					SetSelectedEntity();
					m_SceneContext->DeleteEntity(e);
					deleteEntity = false;
				}
			}


		});


		if (RGF::Input::IsMouseButtonDown(RGF_MOUSE_BUTTON_2) && ImGui::IsWindowHovered()) {
			ImGui::OpenPopup("ScenePopup");
		}
		if (RGF::Input::IsMouseButtonDown(RGF_MOUSE_BUTTON_1) && ImGui::IsWindowHovered() && m_SelectedEntity) {
			SetSelectedEntity();
		}
		if (ImGui::BeginPopup("ScenePopup")) {
			if (ImGui::Button("Create Empty Entity")) {
				SetSelectedEntity(m_SceneContext->CreateEntity());
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();

		}


		ImGui::End();



		// display all components that the selected entity has
		ImGui::Begin("Entity Properties");
		if (m_SelectedEntity) {

			if (m_CurrentlySelectedEntity.HasComponent<NameComponent>()) {
				auto& name = m_CurrentlySelectedEntity.GetComponent<NameComponent>().Name;
				char buffer[255];
				memset(buffer, 0, 255);
				memcpy(buffer, name.c_str(), name.length());
				if (ImGui::InputText("##name", buffer, 255)) {
					name = std::string(buffer);
				}
			}


			auto& transformComp = m_CurrentlySelectedEntity.GetComponent<TransformComponent>();


			if (ImGui::TreeNodeEx((void*)((unsigned int)m_CurrentlySelectedEntity | typeid(TransformComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {

				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100);

				auto [Position, Rotation, Scale] = transformComp.DecomposeTransform();
				auto rot = glm::degrees(glm::eulerAngles(Rotation));
				bool updateTransform = false;

				ImGui::Text("Position");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				if (ImGui::DragFloat3("##position", glm::value_ptr(Position), 0.01f)) {
					updateTransform = true;
				}
				LOCK_MOUSE_IF_NEEDED();

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Rotation");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				if(ImGui::DragFloat3("##rot", glm::value_ptr(rot), 0.01f)){
					updateTransform = true;
				}
				LOCK_MOUSE_IF_NEEDED();

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Scale");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				if (ImGui::DragFloat3("##scale", glm::value_ptr(Scale), 0.01f)) {
					updateTransform = true;
				}
				LOCK_MOUSE_IF_NEEDED();

				ImGui::PopItemWidth();
				ImGui::NextColumn();


				ImGui::Columns(1);
				ImGui::TreePop();

				if (updateTransform) {
					transformComp.Transform = glm::translate(glm::mat4(1.0f), Position)
						* glm::toMat4(glm::quat(glm::radians(rot)))
						* glm::scale(glm::mat4(1.0f), Scale);
				}
			}

			if (m_CurrentlySelectedEntity.HasComponent<SpriteRendererComponent>()) {
				ImGui::NewLine();
				ImGui::Separator();
				bool removeComp = false;

				auto& spriteComp = m_CurrentlySelectedEntity.GetComponent<SpriteRendererComponent>();
				if (ImGui::Button("X", {20, 20})) {
					removeComp = true;
				}
				ImGui::SameLine();
				if (ImGui::TreeNodeEx((void*)((unsigned int)m_CurrentlySelectedEntity | typeid(SpriteRendererComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer")) {


					if (ImGui::BeginPopupContextItem()) {
						if (ImGui::Button("Delete Component"))
							removeComp = true;
						ImGui::EndPopup();
					}

					const auto& spriteRect = spriteComp.SpriteRect;
					ImGui::Columns(2);
					ImGui::SetColumnWidth(0, 150);


					ImGui::Text("FilePath");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					if (ImGui::Button("open")) {
						nfdchar_t* outPath = NULL;
						nfdresult_t result = NFD_OpenDialog("png", NULL, &outPath);

						if (result == NFD_OKAY) {
							puts("Success!");
							if (!spriteRect) {
								// this creates a texture and does not re use textures if the user decides to load a existing texture
								// that is already loaded in the scene.
								//TODO: Refactor the texture manager class and texture class to fix this issue.
								auto texture = Texture::Create(outPath);
								spriteComp.SpriteRect = CreateRef<TextureBounds>(texture, glm::u32vec4(0, 0, texture->GetWidth(), texture->GetHeight()));
								spriteComp.TintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
							}
							else {
								spriteRect->GetTexture()->SetData(outPath);
								spriteRect->SetBounds({ 0, 0, spriteRect->GetTexture()->GetWidth(), spriteRect->GetTexture()->GetHeight() });
							}

							free(outPath);
						}
						else if (result == NFD_CANCEL) {
							RGF_CORE_MSG("User pressed cancel.\n");
						}
						else {
							RGF_CORE_ERROR("Error: %s\n", NFD_GetError());
						}

					}
					ImGui::SameLine();

					if (spriteRect)
						ImGui::InputText("##spriteFilepath", (char*)spriteRect->GetTexture()->GetFilepath().c_str(), 256, ImGuiInputTextFlags_ReadOnly);
					else
						ImGui::InputText("##spriteFilepath", (char*)"No path...", 256, ImGuiInputTextFlags_ReadOnly);

					ImGui::PopItemWidth();


					ImGui::NextColumn();


					ImGui::Text("Sprite coordinates");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					if (spriteRect) {
						ImGui::DragInt4("##spriteRect", (int*)glm::value_ptr(spriteRect->GetBounds()), 1.0f, 0);
						LOCK_MOUSE_IF_NEEDED();
						spriteRect->SetBounds(spriteRect->GetBounds());
					}
					else {
						ImGui::Text("No texture loaded");
					}

					ImGui::PopItemWidth();
					ImGui::NextColumn();

					ImGui::Text("Tint color");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ImGui::ColorEdit4("##tintcolor", glm::value_ptr(spriteComp.TintColor));
					LOCK_MOUSE_IF_NEEDED();

					ImGui::PopItemWidth();
					ImGui::NextColumn();




					ImGui::Text("Texture");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					if (spriteRect) {
						ImGui::Image((void*)spriteComp.SpriteRect->GetTexture()->GetHandleID(), { 128, 128 }, { 0, 1 }, { 1, 0 });
					}
					else {
						ImGui::Text("No texture loaded");
					}
					ImGui::PopItemWidth();
					ImGui::NextColumn();


					ImGui::Text("Sprite rect");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					if (spriteRect) {
						ImGui::Image((void*)spriteComp.SpriteRect->GetTexture()->GetHandleID(), { 128, 128 },
							{ spriteRect->GetBoundsNormilized()[0].x, spriteRect->GetBoundsNormilized()[2].y },
							{ spriteRect->GetBoundsNormilized()[2].x, spriteRect->GetBoundsNormilized()[0].y });
					}
					else {
						ImGui::Text("No texture loaded");
					}

					ImGui::PopItemWidth();
					ImGui::NextColumn();

					ImGui::Columns(1);
					ImGui::TreePop();

				}

				if (removeComp)
					m_CurrentlySelectedEntity.RemoveComponent<SpriteRendererComponent>();

				
			}


			ImGui::NewLine();
			ImGui::Separator();
			if (ImGui::Button("Add component", { 200, 50 })) {
				ImGui::OpenPopup("AddComponentPopup");
			}

			if (ImGui::BeginPopup("AddComponentPopup")) {
				if (!m_CurrentlySelectedEntity.HasComponent<SpriteRendererComponent>()) {

					if (ImGui::Button("Sprite Renderer")) {
						m_CurrentlySelectedEntity.AddComponent <SpriteRendererComponent>();
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}


		}
		ImGui::End();

	}
}