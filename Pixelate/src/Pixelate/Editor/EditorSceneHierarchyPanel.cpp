#include "PXpch.h"
#include "EditorSceneHierarchyPanel.h"

#include <Imgui/imgui.h>
#include "glm/gtc/type_ptr.inl"
#include "Pixelate/Core/MouseButtonCodes.h"
#include "Pixelate/Core/Input.h"


// TODO: Fix this filepath and implement this properly using premake
#include "../../Pixelate-Editor/vendor/NativeFileDialog/src/include/nfd.h"

#include "Pixelate/Rendering/Renderer2D.h"
#include "Pixelate/Imgui/ImGuizmo.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "../Core/KeyCodes.h"

#include "Pixelate/Rendering/RenderCommand.h"
#include "Pixelate/Audio/Audio.h"

namespace Pixelate {

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
	void EditorSceneHierarchyPanel::OnUpdate(float ts, const Ref<EditorCamera>& camera)
	{
		Renderer2D::BeginScene(camera.get());
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


			if (entity.HasComponent<BoxColliderComponent>()) {
				auto& bcc = entity.GetComponent<BoxColliderComponent>();
				auto& transformComp = entity.GetComponent<TransformComponent>();

				AABB boundingBox;
				auto [pos, rot, s] = transformComp.DecomposeTransform();
				glm::vec2 transformPos = { pos.x, pos.y };
				const auto Position = bcc.Center + transformPos;
				const auto Scale = bcc.Size;

				boundingBox.Min = { -Scale.x + Position.x, -Scale.y+ Position.y, 1.0f };
				boundingBox.Max = { Scale.x + Position.x, Scale.y + Position.y, 1.0f };

				Renderer2D::DrawAABB(boundingBox, { 0.0f, 1.0f, 0.0f, 1.0f });
			}


		}

		Renderer2D::EndScene();

	}

	void EditorSceneHierarchyPanel::OnEvent()
	{

	}


	template<typename T, typename lamda>
	static void DrawEntityComponents(const std::string& compName, Entity entity, lamda func) {
		if (entity.HasComponent<T>()) {
			ImGui::NewLine();
			ImGui::Separator();


			auto& comp = entity.GetComponent<T>();
			if (ImGui::Button("X", { 20, 20 })) {
				entity.RemoveComponent<T>();
				return;
			
			}

			ImGui::SameLine();
			bool isOpen = ImGui::TreeNodeEx((void*)((unsigned int)entity | typeid(T).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, compName.c_str());

			
			if (isOpen) {
				func(entity.GetComponent<T>());
				ImGui::NextColumn();
				ImGui::Columns(1);
				ImGui::TreePop();
			}
			ImGui::Separator();

			

		}
	}

	static std::string ConvertBodyTypeEnumToString(Pixelate::BodyType type) {

		switch (type)
		{
			case Pixelate::BodyType::Static:
				return "Static";
			case Pixelate::BodyType::Kinematic:
				return "Kinematic";
			case Pixelate::BodyType::Dynamic:
				return "Dynamic";
		}

		PX_ASSERT(false, "");
		return "null";
	}

	static std::string ConvertSleepStateEnumToString(Pixelate::SleepingState type) {

		switch (type)
		{
		case Pixelate::SleepingState::Awake:
			return "Awake";
		case Pixelate::SleepingState::NeverSleep:
			return "Never Sleep";
		case Pixelate::SleepingState::Sleep:
			return "Sleep";
		}

		PX_ASSERT(false, "");
		return "null";
	}

	void EditorSceneHierarchyPanel::OnImguiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		// display all entities in hierarchy
		bool deleteEntity = false;
		m_SceneContext->GetReg().each([&](auto entity) {

			Entity e(entity, m_SceneContext.get());
			if (e.HasComponent<NameComponent>()) {
				std::string name = e.GetComponent<NameComponent>().Name;

				ImGuiTreeNodeFlags node_flags = ((e == m_CurrentlySelectedEntity) && m_SelectedEntity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
				if (ImGui::TreeNodeEx((void*)(unsigned int)e, node_flags, name.c_str())) {
					ImGui::TreePop();
				}

				if (ImGui::IsItemClicked()) {
					SetSelectedEntity(e);
				}

				if (Input::IsKeyDown(KeyCode::Deletekey))
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

			}


		});


		if (Pixelate::Input::IsMouseButtonDown(MouseButton::Right) && ImGui::IsWindowHovered()) {
			ImGui::OpenPopup("ScenePopup");
		}
		if (Pixelate::Input::IsMouseButtonDown(MouseButton::Left) && ImGui::IsWindowHovered() && m_SelectedEntity) {
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
		ImGui::Begin("Entity Components");
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
			auto [transformPosition, transformRotation, transformScale] = transformComp.DecomposeTransform();
			if (ImGui::TreeNodeEx((void*)((unsigned int)m_CurrentlySelectedEntity | typeid(TransformComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {

				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100);

				auto rot = glm::degrees(glm::eulerAngles(transformRotation));
				bool updateTransform = false;

				ImGui::Text("Position");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				if (ImGui::DragFloat3("##position", glm::value_ptr(transformPosition), 0.01f)) {
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
				if (ImGui::DragFloat3("##scale", glm::value_ptr(transformScale), 0.01f)) {
					updateTransform = true;
				}
				LOCK_MOUSE_IF_NEEDED();

				ImGui::PopItemWidth();
				ImGui::NextColumn();


				ImGui::Columns(1);
				ImGui::TreePop();

				if (updateTransform) {

					transformComp.Transform = glm::translate(glm::mat4(1.0f), transformPosition)
						* glm::toMat4(glm::quat(glm::radians(rot)))
						* glm::scale(glm::mat4(1.0f), transformScale);
				}
			}

			DrawEntityComponents<SpriteRendererComponent>("Sprite Renderer", m_CurrentlySelectedEntity, [](SpriteRendererComponent& spriteComp) {
				const auto& spriteRect = spriteComp.SpriteRect;
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);


				ImGui::Text("FilePath");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				if (ImGui::Button("open")) {
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("png,jpeg,jpg", NULL, &outPath);

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
						PX_CORE_MSG("User pressed cancel.\n");
					}
					else {
						PX_CORE_ERROR("Error: %s\n", NFD_GetError());
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
			});
		
			DrawEntityComponents<CameraComponent>("Orthographic Camera", m_CurrentlySelectedEntity, [](CameraComponent& cc) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);

				ImGui::Text("Is Primary");
				ImGui::NextColumn();
				ImGui::Checkbox("##PrCamera", &cc.PrimaryCamera);
				ImGui::NextColumn();
				ImGui::Text("Orthographic Scale");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				float orthoSize = cc.Camera.GetOrthographicSize();
				ImGui::DragFloat("##orthoSize", &orthoSize, 0.01f);
				LOCK_MOUSE_IF_NEEDED();
				cc.Camera.SetOrthographicSize(orthoSize);
				ImGui::PopItemWidth();
				ImGui::NextColumn();
				ImGui::Text("Clear Color");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::ColorEdit4("##ClearColor", glm::value_ptr(cc.ClearColor));
				LOCK_MOUSE_IF_NEEDED();
				ImGui::PopItemWidth();
				ImGui::NextColumn();
			});
			DrawEntityComponents<RigidBodyComponent>("RigidBody", m_CurrentlySelectedEntity, [](RigidBodyComponent& rbc) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);



				ImGui::Text("Freeze Z rotation");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::Checkbox("##FreezeZRotCheckmark", &rbc.Definition.CanRotate);
				ImGui::PopItemWidth();
				ImGui::NextColumn();


				ImGui::Text("Gravity scale");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::DragFloat("##gravScale", &rbc.Definition.GravityScale);
				ImGui::PopItemWidth();
				ImGui::NextColumn();





				ImGui::Text("Type");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);




				Pixelate::BodyType currentType = rbc.Definition.Type;

				if (ImGui::BeginCombo("##Type", ConvertBodyTypeEnumToString(currentType).c_str()))
				{
					bool dyIsSelected = (currentType == BodyType::Dynamic);
					bool stIsSelected = (currentType == BodyType::Static);
					bool kiIsSelected = (currentType == BodyType::Kinematic);

					if (ImGui::Selectable("Dynamic", dyIsSelected))
						currentType = BodyType::Dynamic;

					if (ImGui::Selectable("Static", stIsSelected))
						currentType = BodyType::Static;

					if (ImGui::Selectable("Kinematic", kiIsSelected))
						currentType = BodyType::Kinematic;

					ImGui::EndCombo();
				}
				
				rbc.Definition.Type = currentType;
				ImGui::PopItemWidth();
				ImGui::NextColumn();



				ImGui::Text("Initial sleep state");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				Pixelate::SleepingState currentState = rbc.Definition.State;

				if (ImGui::BeginCombo("##State", ConvertSleepStateEnumToString(currentState).c_str()))
				{
					bool aIsSelected = (currentState == SleepingState::Awake);
					bool sIsSelected = (currentState == SleepingState::Sleep);
					bool nsIsSelected = (currentState == SleepingState::NeverSleep);

					if (ImGui::Selectable("Awake", aIsSelected))
						currentState = SleepingState::Awake;

					if (ImGui::Selectable("Sleep", sIsSelected))
						currentState = SleepingState::Sleep;

					if (ImGui::Selectable("NeverSleep", nsIsSelected))
						currentState = SleepingState::NeverSleep;

					ImGui::EndCombo();
				}

				rbc.Definition.State = currentState;


				ImGui::PopItemWidth();
				ImGui::NextColumn();


			

			});

			DrawEntityComponents<BoxColliderComponent>("Box Collider", m_CurrentlySelectedEntity, [](BoxColliderComponent& bcc) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);

				ImGui::Text("Center");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::DragFloat2("##center", glm::value_ptr(bcc.Center), 0.1f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();
				ImGui::Text("Size");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::DragFloat2("##size", glm::value_ptr(bcc.Size), 0.1f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();
				ImGui::Text("Is trigger");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::Checkbox("##istrigger", &bcc.IsTrigger);
				ImGui::PopItemWidth();

			});

			DrawEntityComponents<AudioSourceComponent>("Audio Source", m_CurrentlySelectedEntity, [](AudioSourceComponent& asc) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);

				ImGui::Text("Source filepath");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				if (ImGui::Button("open")) {
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("wav,ogg,mp3", NULL, &outPath);
					if (result == NFD_OKAY) {
						puts("Success!");

						asc.Source = Audio::CreateAudioSource(outPath);
						asc.FilePath = outPath;

						free(outPath);
					} else if (result == NFD_CANCEL) {
						PX_CORE_MSG("User pressed cancel.\n");
					} else {
						PX_CORE_ERROR("Error: %s\n", NFD_GetError());
					}
				}

				ImGui::SameLine();

				if (asc.Source)
					ImGui::InputText("##sourceFilepath", (char*)asc.FilePath.c_str(), 256, ImGuiInputTextFlags_ReadOnly);
				else
					ImGui::InputText("##sourceFilepath", (char*)"No path...", 256, ImGuiInputTextFlags_ReadOnly);

				//ImGui::DragFloat2("##center", glm::value_ptr(bcc.Center), 0.1f);
				ImGui::PopItemWidth();
				});

			DrawEntityComponents<AudioListenerComponent>("Audio Listener", m_CurrentlySelectedEntity, [](AudioListenerComponent& alc) {
			
			});
			DrawEntityComponents<ScriptingBehaviourComponent>("Scripting Behaviour", m_CurrentlySelectedEntity, [](ScriptingBehaviourComponent& sbc) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);

				ImGui::Text("Script name");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				char buffer[255];
				memset(buffer, 0, 255);
				memcpy(buffer, sbc.Behaviour.ClassName.c_str(), sbc.Behaviour.ClassName.length());
				if (ImGui::InputText("##scriptName", buffer, 255)) {
					sbc.Behaviour.ClassName = std::string(buffer);

					if(ScriptingMaster::ClassExists(sbc.Behaviour.ClassName))
						ScriptingMaster::CreateEntityScript(sbc.Behaviour);


				}
				//ImGui::DragFloat2("##center", glm::value_ptr(bcc.Center), 0.1f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();
				ImGui::Text("Size");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				//ImGui::DragFloat2("##size", glm::value_ptr(bcc.Size), 0.1f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();
				ImGui::Text("Is trigger");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
//				ImGui::Checkbox("##istrigger", &bcc.IsTrigger);
				ImGui::PopItemWidth();

				});



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
				if (!m_CurrentlySelectedEntity.HasComponent<CameraComponent>()) {

					if (ImGui::Button("Camera")) {
						auto& cam = m_CurrentlySelectedEntity.AddComponent <CameraComponent>();
						ImGui::CloseCurrentPopup();
					}


				}
				if (!m_CurrentlySelectedEntity.HasComponent<RigidBodyComponent>()) {

					if (ImGui::Button("RigidBody")) {
						m_CurrentlySelectedEntity.AddComponent <RigidBodyComponent>();
						ImGui::CloseCurrentPopup();
					}

				}
				if (!m_CurrentlySelectedEntity.HasComponent<BoxColliderComponent>()) {

					if (ImGui::Button("Box Collider")) {
						m_CurrentlySelectedEntity.AddComponent <BoxColliderComponent>(glm::vec2(0.0f, 0.0f), glm::vec2(transformScale.x / 2, transformScale.y /2 ), false);
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_CurrentlySelectedEntity.HasComponent<AudioSourceComponent>()) {

					if (ImGui::Button("Audio Source")) {
						m_CurrentlySelectedEntity.AddComponent <AudioSourceComponent>();
						ImGui::CloseCurrentPopup();
					}
				}
				if (!m_CurrentlySelectedEntity.HasComponent<AudioListenerComponent>()) {

					if (ImGui::Button("Audio Listener")) {
						m_CurrentlySelectedEntity.AddComponent <AudioListenerComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_CurrentlySelectedEntity.HasComponent<ScriptingBehaviourComponent>()) {

					if (ImGui::Button("Scripting Behaviour")) {
						m_CurrentlySelectedEntity.AddComponent <ScriptingBehaviourComponent>();
						ImGui::CloseCurrentPopup();
					}
				}
				ImGui::EndPopup();
			}


		}
		ImGui::End();

	}
}