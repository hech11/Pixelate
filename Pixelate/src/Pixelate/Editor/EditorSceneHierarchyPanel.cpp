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
		m_SelectedEntity = false;
	}
	void EditorSceneHierarchyPanel::SetSelectedEntity(Entity entity) {
		m_CurrentlySelectedEntity = entity;
		m_SelectedEntity = true;
	}
	void EditorSceneHierarchyPanel::SetSelectedEntity() {
		m_CurrentlySelectedEntity = {};
		m_SelectedEntity = false;
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

	static std::string ConvertColDetectionModeEnumToString(Pixelate::CollisionDetectionMode mode) {
		if (mode == CollisionDetectionMode::Continuous)
			return "Continuous";
		else
			return "Discrete";
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


			if (m_CurrentlySelectedEntity.HasComponent<UUIDComponent>()) {
				auto& id = m_CurrentlySelectedEntity.GetComponent<UUIDComponent>().UUID;
				
				ImGui::TextDisabled(std::to_string(id).c_str());
			}


			if (m_CurrentlySelectedEntity.HasComponent<NameComponent>()) {
				auto& name = m_CurrentlySelectedEntity.GetComponent<NameComponent>().Name;
				char buffer[255];
				memset(buffer, 0, 255);
				memcpy(buffer, name.c_str(), name.length());
				if (ImGui::InputText("##name", buffer, 255)) {
					name = std::string(buffer);
				}
				ImGui::SameLine();
			}


			if (m_CurrentlySelectedEntity.HasComponent<TagComponent>()) {
				auto& tag = m_CurrentlySelectedEntity.GetComponent<TagComponent>().Tag;
				char buffer[255];
				memset(buffer, 0, 255);
				memcpy(buffer, tag.c_str(), tag.length());
				if (ImGui::InputText("##tag", buffer, 255)) {
					tag = std::string(buffer);
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
				const auto& spriteRect = spriteComp.Texture;
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);


				ImGui::Text("FilePath");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				if (ImGui::Button("open")) {
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("png,jpeg,jpg", NULL, &outPath);

					if (result == NFD_OKAY) {
						PX_CORE_TRACE("Success!");

						Ref<Texture> tex;
						if (auto id = TextureManager::IsTextureValid(outPath)) {
							tex = TextureManager::GetTexture(id);
						} else {
							tex = Texture::Create(outPath);
							TextureManager::DirectAdd(tex);
						}
						spriteComp.Texture = tex;
						spriteComp.Rect = Rect({ 0.0f, 0.0f }, { tex->GetWidth(), tex->GetHeight() });
						spriteComp.TintColor = { 1.0f, 1.0f, 1.0f, 1.0f };


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
					ImGui::InputText("##spriteFilepath", (char*)spriteRect->GetFilepath().c_str(), 256, ImGuiInputTextFlags_ReadOnly);
				else
					ImGui::InputText("##spriteFilepath", (char*)"No path...", 256, ImGuiInputTextFlags_ReadOnly);

				ImGui::PopItemWidth();




				

				ImGui::NextColumn();
				ImGui::Text("Tint color");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::ColorEdit4("##tintcolor", glm::value_ptr(spriteComp.TintColor));
				LOCK_MOUSE_IF_NEEDED();

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				if (spriteRect) {
					ImGui::Text("Sprite coordinates");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ImGui::DragInt4("##spriteRect", (int*)glm::value_ptr(spriteComp.Rect.Position), 1.0f, 0);
					LOCK_MOUSE_IF_NEEDED();

					ImGui::PopItemWidth();
					ImGui::NextColumn();

				}


				if (spriteRect) {
					ImGui::Text("Texture");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);

					ImGui::Image((void*)spriteRect->GetHandleID(), { 128, 128 }, { 0, 1 }, { 1, 0 });
				
					ImGui::PopItemWidth();
					ImGui::NextColumn();
				}


				if (spriteRect) {
					ImGui::Text("Sprite rect");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					//ImGui::Image((void*)spriteComp.SpriteRect->GetTexture()->GetHandleID(), { 128, 128 },
						//{ spriteRect->GetBoundsNormilized()[0].x, spriteRect->GetBoundsNormilized()[2].y },
						//{ spriteRect->GetBoundsNormilized()[2].x, spriteRect->GetBoundsNormilized()[0].y });

					ImGui::PopItemWidth();
				}
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
				ImGui::Checkbox("##FreezeZRotCheckmark", &rbc.Definition.FixedRotation);
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


				ImGui::Text("Collision Detection Mode");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				Pixelate::CollisionDetectionMode currentMode = rbc.Definition.DetectionMode;

				if (ImGui::BeginCombo("##Mode", ConvertColDetectionModeEnumToString(currentMode).c_str()))
				{
					bool cIsSelected = (currentMode == CollisionDetectionMode::Continuous);
					bool dIsSelected = (currentMode == CollisionDetectionMode::Discrete);

					if (ImGui::Selectable("Continuous", cIsSelected))
						currentMode = CollisionDetectionMode::Continuous;

					if (ImGui::Selectable("Discrete", dIsSelected))
						currentMode = CollisionDetectionMode::Discrete;

					ImGui::EndCombo();
				}

				rbc.Definition.DetectionMode = currentMode;


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
						PX_CORE_TRACE("Success!");

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

				ImGui::PopItemWidth();

				if (asc.Source) {
					ImGui::NextColumn();
					ImGui::Text("Should Loop");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					bool loop = asc.Source->IsLooping();
					if (ImGui::Checkbox("##ShouldLoop", &loop)) {
						asc.Source->SetLooping(loop);
					}
					ImGui::PopItemWidth();

					ImGui::NextColumn();
					ImGui::Text("Gain");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					float gain = asc.Source->GetGain();
					if (ImGui::DragFloat("##SetGain", &gain)) {
						asc.Source->SetGain(gain);
					}
					ImGui::PopItemWidth();

				}



				});

			DrawEntityComponents<AudioListenerComponent>("Audio Listener", m_CurrentlySelectedEntity, [](AudioListenerComponent& alc) {


				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);

				ImGui::Text("Spatial");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				bool sAudio = alc.Listener.IsSpatial();
				if (ImGui::Checkbox("##SpatialAudio", &sAudio)) {
					alc.Listener.SetSpatial(sAudio);
				}
				ImGui::PopItemWidth();


			});
			DrawEntityComponents<ScriptingBehaviourComponent>("Scripting Behaviour", m_CurrentlySelectedEntity, [&](ScriptingBehaviourComponent& sbc) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);

				ImGui::Text("Script name");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				char buffer[255];
				memset(buffer, 0, 255);
				memcpy(buffer, sbc.Behaviour.ClassName.c_str(), sbc.Behaviour.ClassName.length());

				if(!ScriptingMaster::ClassExists(buffer)) {
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, { 1, 0, 0,1 });
				} else
					ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, { 1, 1, 1,1 });

				if (ImGui::InputText("##scriptName", buffer, 255)) {
					sbc.Behaviour.ClassName = std::string(buffer);

					if (ScriptingMaster::ClassExists(sbc.Behaviour.ClassName)) {
						ScriptingMaster::CreateEntityScript(m_CurrentlySelectedEntity ,sbc.Behaviour);
					} 

				}
				
				ImGui::PopStyleColor();
				ImGui::PopItemWidth();
				
				if (ScriptingMaster::ClassExists(sbc.Behaviour.ClassName)) {
					// Draw all the scripts public variables here
					for (unsigned int i = 0; i < sbc.Behaviour.AllFields.size(); i++) {
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);

						ImGui::Text(sbc.Behaviour.AllFields[i].VariableName.c_str());

						ImGui::PopItemWidth();
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);

						
						switch (sbc.Behaviour.AllFields[i].Type)
						{
						case PropertyType::Bool: {
							sbc.Behaviour.AllFields[i].Value = sbc.Behaviour.GetFieldValue(sbc.Behaviour.AllFields[i].Field);
							bool tempBool = (bool)sbc.Behaviour.AllFields[i].Value;
							if (ImGui::Checkbox("##Bool", &tempBool)) {
								sbc.Behaviour.AllFields[i].Value = (bool*)std::move(tempBool);
								sbc.Behaviour.SetPropertyValue(&tempBool, sbc.Behaviour.AllFields[i].Field);
							}
						}
							break;
						case PropertyType::Float: {
							sbc.Behaviour.AllFields[i].Value = sbc.Behaviour.GetFieldValue(sbc.Behaviour.AllFields[i].Field);
							float tempFloat = *(float*)&sbc.Behaviour.AllFields[i].Value;
							if(ImGui::InputFloat("##Float", &tempFloat)) {
								sbc.Behaviour.AllFields[i].Value = std::move(&tempFloat);
								sbc.Behaviour.SetPropertyValue(&tempFloat, sbc.Behaviour.AllFields[i].Field);
							}
						}
							break;
						case PropertyType::Int: {
							sbc.Behaviour.AllFields[i].Value = sbc.Behaviour.GetFieldValue(sbc.Behaviour.AllFields[i].Field);
							int tempInt = (int)sbc.Behaviour.AllFields[i].Value;
							if (ImGui::InputInt("##Int", &tempInt)) {
								sbc.Behaviour.AllFields[i].Value = (int*)std::move(tempInt);
								sbc.Behaviour.SetPropertyValue(&tempInt, sbc.Behaviour.AllFields[i].Field);
							}
							break;
						}
						case PropertyType::Vec2:
							break;
						case PropertyType::Vec3:
							break;
						case PropertyType::Vec4:
							break;
						default:
							break;
						}

						ImGui::PopItemWidth();

					}
				}
				


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