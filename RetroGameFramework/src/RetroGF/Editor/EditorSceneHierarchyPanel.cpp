#include "RGFpch.h"
#include "EditorSceneHierarchyPanel.h"

#include <Imgui/imgui.h>
#include "glm/gtc/type_ptr.inl"


// TODO: Fix this filepath and implement this properly using premake
#include "../../Retro-Editor/vendor/NativeFileDialog/src/include/nfd.h"

namespace RGF {



	EditorSceneHierarchyPanel::EditorSceneHierarchyPanel(const Ref<Scene>& scene)
		:m_SceneContext(scene)
	{

	}

	void EditorSceneHierarchyPanel::SetSceneContext(const Ref<Scene>& scene)
	{
		m_SceneContext = scene;
	}
	void EditorSceneHierarchyPanel::SetSelectedEntity(const Ref<Entity>& entity) {
		m_CurrentlySelectedEntity = *entity.get();
	}

	void EditorSceneHierarchyPanel::OnUpdate(float ts)
	{

	}

	void EditorSceneHierarchyPanel::OnEvent()
	{

	}

	void EditorSceneHierarchyPanel::OnImguiRender()
	{
		static bool selectedEntity = false;
		ImGui::Begin("Scene Hierarchy");

		// display all entities in hierarchy
		m_SceneContext->GetReg().each([&](auto entity) {

			Entity e(entity, m_SceneContext.get());
			std::string name = "Entity";

			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
			if (ImGui::TreeNodeEx((void*)(unsigned int)e, node_flags, name.c_str())) {
				ImGui::TreePop();
			}

			if (ImGui::IsItemClicked()) {
				m_CurrentlySelectedEntity = e;
				selectedEntity = true;
			}

		});
		ImGui::End();






		// display all components that the selected entity has
		ImGui::Begin("Entity Properties");
		if (selectedEntity) {
			auto& transformComp = m_CurrentlySelectedEntity.GetComponent<TransformComponent>();
			if (ImGui::TreeNodeEx((void*)(unsigned int)m_CurrentlySelectedEntity, ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {

				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100);


				ImGui::Text("Position");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::DragFloat3("##position", glm::value_ptr(transformComp.Position), 0.1f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Rotation");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::DragFloat("##rot", &transformComp.Rotation, 0.1f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Scale");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::DragFloat3("##scale", glm::value_ptr(transformComp.Scale), 0.1f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Columns(1);
				ImGui::TreePop();
			}
			ImGui::Separator();

			auto& spriteComp = m_CurrentlySelectedEntity.GetComponent<SpriteRendererComponent>();
			if (ImGui::TreeNodeEx((void*)((unsigned int)m_CurrentlySelectedEntity | typeid(SpriteRendererComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer")) {
				const auto& texture = spriteComp.SpriteRect->GetTexture();
				const auto& spriteRect = spriteComp.SpriteRect;

				ImGui::Columns(3);
				ImGui::SetColumnWidth(0, 100);
				ImGui::SetColumnWidth(1, 300);
				ImGui::SetColumnWidth(2, 40);


				ImGui::Text("FilePath");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::InputText("##spriteFilepath", (char*)spriteRect->GetTexture()->GetFilepath().c_str(), 256, ImGuiInputTextFlags_ReadOnly);
				ImGui::PopItemWidth();
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				if (ImGui::SmallButton("open")) {
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("png", NULL, &outPath);

					if (result == NFD_OKAY) {
						puts("Success!");
						spriteRect->GetTexture()->SetData(outPath);
						free(outPath);
					}
					else if (result == NFD_CANCEL) {
						RGF_CORE_MSG("User pressed cancel.\n");
					}
					else {
						RGF_CORE_ERROR("Error: %s\n", NFD_GetError());
					}

				}
				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Columns(2);


				ImGui::Text("Texture");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::Image((void*)spriteComp.SpriteRect->GetTexture()->GetHandleID(), { 128, 128 }, { 0, 1 }, { 1, 0 });
				ImGui::PopItemWidth();
				ImGui::NextColumn();


				ImGui::Text("Sprite rect");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::Image((void*)texture->GetHandleID(), { 128, 128 },
					{ spriteRect->GetBoundsNormilized()[0].x, spriteRect->GetBoundsNormilized()[2].y },
					{ spriteRect->GetBoundsNormilized()[2].x, spriteRect->GetBoundsNormilized()[0].y });
				ImGui::PopItemWidth();
				ImGui::NextColumn();


				ImGui::Text("Sprite coordinates");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::DragInt4("##spriteRect", (int*)glm::value_ptr(spriteRect->GetBounds()), 1.0f, 0);
				spriteRect->SetBounds(spriteRect->GetBounds());
				ImGui::PopItemWidth();
				ImGui::NextColumn();



				ImGui::Columns(1);
				ImGui::TreePop();
			}

		}


		ImGui::End();


	}

}