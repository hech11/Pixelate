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
#include "EditorTextureInspector.h"
#include <Imgui\imgui_internal.h>
#include "Pixelate/Asset/AssetManager.h"
#include "Pixelate/Asset/Asset.h"
#include "Pixelate/Physics/Physics.h"

#include "Pixelate/Rendering/API/Shader/ShaderLibrary.h"
#include "Pixelate/Rendering/MaterialManager.h"

#include "EditorPanel.h"
namespace Pixelate {

#define LOCK_MOUSE_IF_NEEDED() if (ImGui::IsItemActivated()) {\
	Input::SetMouseLockMode(Input::MouseLockMode::Locked);\
}\
if (ImGui::IsItemDeactivated()) {\
Input::SetMouseLockMode(Input::MouseLockMode::None);\
}

	EditorSceneHierarchyPanel::EditorSceneHierarchyPanel(const Ref<Scene>& scene)
	{
		m_SceneContext = scene;
	}

	


	template<typename T, typename lamda>
	static void DrawEntityComponents(const std::string& compName, Entity entity, lamda func) {
		if (entity.HasComponent<T>()) {
			ImGui::NewLine();
			ImGui::Separator();


			auto& comp = entity.GetComponent<T>();
			ImGui::PushID(compName.c_str());
			if (ImGui::Button("X", { 20, 20 })) {
				entity.RemoveComponent<T>();
				ImGui::PopID();
				return;
			}
			ImGui::PopID();

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


	template<typename lamda>
	static void BeginDragDrop(lamda func) {

		ImVec2 vMin = ImGui::GetItemRectMin();
		ImVec2 vMax = ImGui::GetItemRectMax();

		ImRect dragAreaRect = { vMin , vMax };
		auto viewportID = ImGui::GetID("Entity Components");

		if (ImGui::BeginDragDropTargetCustom(dragAreaRect, viewportID)) {
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("AssetPayload"))
			{
				AssetMetadata& metadata = *(AssetMetadata*)payload->Data;
				std::filesystem::path filepath = AssetManager::GetFilePath(metadata);

				func(metadata);

			}
			ImGui::EndDragDropTarget();
		}

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

				ImGuiTreeNodeFlags node_flags = ((e == m_CurrentlySelectedEntity) && m_CurrentlySelectedEntity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
				if (ImGui::TreeNodeEx((void*)(unsigned int)e, node_flags, name.c_str())) {
					ImGui::TreePop();
				}

				if (ImGui::IsItemClicked()) {
					EditorPanelManager::Get().SetSelectedEntity(e);
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
						EditorPanelManager::Get().SetSelectedEntity({});
						m_SceneContext->DeleteEntity(e);
						deleteEntity = false;
					}
				}

			}


		});


		if (Pixelate::Input::IsMouseButtonDown(MouseButton::Right) && ImGui::IsWindowHovered()) {
			ImGui::OpenPopup("ScenePopup");
		}
		if (Pixelate::Input::IsMouseButtonDown(MouseButton::Left) && ImGui::IsWindowHovered() && m_HasSelectedEntity) {
			SetSelectedEntity({});
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
		if (m_HasSelectedEntity) {


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

			DrawEntityComponents<SpriteRendererComponent>("Sprite Renderer", m_CurrentlySelectedEntity, [&](SpriteRendererComponent& spriteComp) {
				const auto& spriteRect = spriteComp.Texture;
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);


				/*ImGui::Text("FilePath");



				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::PushID(1);
				if (ImGui::Button("open")) {
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("png,jpeg,jpg", NULL, &outPath);

					if (result == NFD_OKAY) {
						PX_CORE_TRACE("Success!\n");

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


				BeginDragDrop([&](AssetMetadata& metadata) {
					if (metadata.Type == AssetType::Texture) {
						spriteComp.Texture = AssetManager::GetAsset<Texture>(metadata.Handle);
						spriteComp.Rect = { {0, 0}, {spriteComp.Texture->GetWidth(), spriteComp.Texture->GetHeight()} };

					}
				});

				ImGui::PopID();

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				*/
				
				/*
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
				*/


				ImGui::Text("Sorting Layer");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				RenderLayer sortingLayer = spriteComp.SortingLayer;
				if (ImGui::BeginCombo("##SortingLayer", SortingLayerManager::GetLayerName(sortingLayer).c_str())) {

					for (auto& layer : SortingLayerManager::GetLayers()) {
						if (ImGui::Selectable(layer.second.c_str(), layer.first == sortingLayer)) {
							sortingLayer = layer.first;
						}
					}

					spriteComp.SortingLayer = sortingLayer;

					m_SceneContext->GetReg().sort<SpriteRendererComponent>([](const SpriteRendererComponent& lhs, const SpriteRendererComponent& rhs)
						{
							return lhs.RenderOrder + (1 + lhs.SortingLayer * 100) < rhs.RenderOrder + (1 + rhs.SortingLayer * 100);
						});

					ImGui::EndCombo();
				}


				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Order in Layer");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				if (ImGui::InputInt("##orderinlayer", &spriteComp.RenderOrder))
				{
					m_SceneContext->GetReg().sort<SpriteRendererComponent>([](const SpriteRendererComponent& lhs, const SpriteRendererComponent& rhs)
						{
							return lhs.RenderOrder + (1 + lhs.SortingLayer * 100) < rhs.RenderOrder + (1 + rhs.SortingLayer * 100);
						});
				}

				ImGui::PopItemWidth();
				ImGui::NextColumn();


				ImGui::Text("Material path");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::PushID(2);
				if (ImGui::Button("open")) {
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("pxShader", NULL, &outPath);

					if (result == NFD_OKAY) {
						PX_CORE_TRACE("Success!\n");

						auto path = std::filesystem::relative(outPath, "assets/");

						Ref<Material> mat = MaterialManager::Load(path.string());
						spriteComp.Material = mat;

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


				if (spriteComp.Material)
				{

					//TODO: Handle external resources. We only support assets local to the project!
					std::filesystem::path path;
					if (AssetManager::IsResource(spriteComp.Material->Handle))
					{
						path = AssetManager::GetFilePath(AssetManager::GetMetadata(spriteComp.Material->Handle), true);
					}
					else
					{
						path = AssetManager::GetFilePath(AssetManager::GetMetadata(spriteComp.Material->Handle), false);
					}
					ImGui::InputText("##MaterialFilepath", (char*)path.string().c_str(), 256, ImGuiInputTextFlags_ReadOnly);
				}
				else
				{
					ImGui::InputText("##MaterialFilepath", (char*)"No path...", 256, ImGuiInputTextFlags_ReadOnly);
				}


				BeginDragDrop([&](AssetMetadata& metadata) {
					if (metadata.Type == AssetType::Material) {
						std::filesystem::path path = metadata.Filepath;

						Ref<Material> mat = MaterialManager::Load(path.string());
						spriteComp.Material = mat;
					}
					});
				ImGui::PopID();
				ImGui::PopItemWidth();


				ImGui::Columns(1);
				ImGui::Separator();
				ImGui::PushItemWidth(-1);
				ImGui::SetCursorPos({ ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("Material Editor").x / 2, ImGui::GetCursorPosY()});
				ImGui::Text("Material Editor");

				ImGui::Columns(2);
				ImGui::PushID(2);
				ImGui::Text("Material");
				ImGui::NextColumn();

				bool changedShader = false;
				Ref<Material>& material = m_CurrentlySelectedEntity.GetComponent<SpriteRendererComponent>().Material;
				std::vector<std::string> items;
				items.push_back(material->GetName().c_str());
				std::string current_item = items[0];

				auto metadatas = AssetManager::GetMetadataOfType(AssetType::Material);


				for (auto& metadata : metadatas)
				{

					std::string name = metadata.Filepath.stem().string();
					if (name == items[0])
					{
						continue;
					}
					items.push_back(name);
				}


				if (ImGui::BeginCombo("##Material-list", current_item.c_str()))
				{
					for (auto& item : items) {
						bool is_selected = (current_item == item.c_str());
						if (ImGui::Selectable(item.c_str(), is_selected))
						{
							current_item = item;

							Ref<Material> result = nullptr;
							for (auto& metadata : metadatas)
							{
								std::string name = metadata.Filepath.stem().string();
								if (name == item)
								{
									result = AssetManager::GetAsset<Material>(metadata.Handle);
									break;
								}
							}
							material = result;


							changedShader = true;
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				ImGui::PopID();


				ImGui::PopItemWidth();

				ImGui::NextColumn();

				for (auto& materialTable: material->GetUniformTable())
				{
					for (auto&& [binding, ubs] : materialTable->ReflectedUniformBuffers)
					{
						for (auto& materialProperty : ubs.Members)
						{
							RenderMaterialProperty(material, materialProperty, ubs.Binding);
						}
					}

				}

				for (auto&& [binding, sampledImages] : material->GetReflectedSampledImages())
				{
					RenderMaterialSampledImage2D(material, sampledImages, binding);
				}

/*				if (spriteRect) {
					ImGui::Text("Texture");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);

					if (ImGui::ImageButton((void*)spriteRect->GetHandleID(), { 128, 128 }, { 0, 1 }, { 1, 0 })) {
						const auto& inspector = std::dynamic_pointer_cast<EditorTextureInspector>(EditorPanelManager::Get().GetPanel("TextureInspector"));

						inspector->SetOpenPanel(true);
						inspector->SetTextureContext(spriteRect);

					}

					BeginDragDrop([&](AssetMetadata& metadata) {
						if (metadata.Type == AssetType::Texture) {
							spriteComp.Texture = AssetManager::GetAsset<Texture>(metadata.Handle);
							spriteComp.Rect = { {0, 0}, {spriteComp.Texture->GetWidth(), spriteComp.Texture->GetHeight()} };

						}
					});
				
					ImGui::PopItemWidth();
					ImGui::NextColumn();
				}
				*/
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





				ImGui::Text("Layer");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);


				int categoryLayer = rbc.Definition.CategoryLayer;

				if (ImGui::BeginCombo("##PhysicsLayer", Physics::LayerIDToString(categoryLayer).c_str())) {

					for (auto& layer : Physics::GetLayers()) {
						if (ImGui::Selectable(layer.DebugName.c_str(), layer.ID == categoryLayer)) {
							categoryLayer = layer.ID;
						}
					}

					rbc.Definition.CategoryLayer = categoryLayer;

					ImGui::EndCombo();
				}



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



				ImGui::Text("Physics Material2D");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				Ref<PhysicsMaterial2D>& material = rbc.Definition.Material;
				const AssetHandle& handle = rbc.Definition.Material->Handle;
				if (handle == 0) {
					ImGui::InputText("##PhysicsMaterial2DPath", (char*)"No Path...", ImGuiInputTextFlags_ReadOnly);
				}
				else {
					ImGui::InputText("##PhysicsMaterial2DPath", (char*)AssetManager::GetFilePathString(AssetManager::GetMetadata(handle)).c_str(), ImGuiInputTextFlags_ReadOnly);
				}


				BeginDragDrop([&](AssetMetadata& metadata) {
					if (metadata.Type == AssetType::PhysicsMaterial2D) {
						material = AssetManager::GetAsset<PhysicsMaterial2D>(metadata.Handle);
					}
				});



				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Mass");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);


				if (ImGui::SliderFloat("##PhysicsMaterial2DMass", &material->Mass, 0.0f, 1.0f)) {
					if (handle != 0) {
						PhysicsMaterial2DSerialization::Serialize(AssetManager::GetFilePathString(AssetManager::GetMetadata(handle)).c_str(), material);
					}
				}

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Friction");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);


				if (ImGui::SliderFloat("##PhysicsMaterial2DFriction", &material->Friction, 0.0f, 1.0f)) {
					if (handle != 0) {
						PhysicsMaterial2DSerialization::Serialize(AssetManager::GetFilePathString(AssetManager::GetMetadata(handle)).c_str(), material);
					}
				}

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Bounciness");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);


				if (ImGui::SliderFloat("##PhysicsMaterial2DBounciness", &material->Bounciness, 0.0f, 1.0f)) {
					if (handle != 0) {
						PhysicsMaterial2DSerialization::Serialize(AssetManager::GetFilePathString(AssetManager::GetMetadata(handle)).c_str(), material);
					}
				}

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
			DrawEntityComponents<CircleColliderComponent>("Circle Collider", m_CurrentlySelectedEntity, [](CircleColliderComponent& cc) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);

				ImGui::Text("Center");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::DragFloat2("##center", glm::value_ptr(cc.Center), 0.1f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();
				ImGui::Text("Radius");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::DragFloat("##rad", &cc.Radius, 0.1f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();
				ImGui::Text("Is trigger");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::Checkbox("##istrigger", &cc.IsTrigger);
				ImGui::PopItemWidth();

			});
			DrawEntityComponents<EdgeColliderComponent>("Edge Collider", m_CurrentlySelectedEntity, [](EdgeColliderComponent& cc) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);

				ImGui::Text("Point A");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::DragFloat2("##Point A", glm::value_ptr(cc.Point1), 0.1f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();
				ImGui::Text("Point B");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::DragFloat2("##Point B", glm::value_ptr(cc.Point2), 0.1f);
				ImGui::PopItemWidth();
				ImGui::NextColumn();
				ImGui::Text("Is trigger");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::Checkbox("##istrigger", &cc.IsTrigger);
				ImGui::PopItemWidth();

			});


			DrawEntityComponents<PolygonColliderComponent>("Polygon Collider", m_CurrentlySelectedEntity, [](PolygonColliderComponent& pcc) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);

				ImGui::Text("Points");
				ImGui::NextColumn();
				for (int i = 0; i < pcc.Vertices.size(); i++) {
					ImGui::PushItemWidth(-1);
					std::string id = "##PointP" + std::to_string(i);
					if (i > 2) {
						if (ImGui::SmallButton("x")) {
							pcc.Vertices.erase(pcc.Vertices.begin() + i);
						}
						ImGui::SameLine();
					}
					ImGui::DragFloat2(id.c_str(), &pcc.Vertices[i].x, 0.1f);
					ImGui::PopItemWidth();
				}
				ImGui::NextColumn();
				ImGui::Text("Add Point");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				if (ImGui::Button("##addpoint")) {
					pcc.Vertices.push_back({ 0.0f, 0.0f });
				}
				ImGui::PopItemWidth();
				ImGui::NextColumn();
				ImGui::Text("Is trigger");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::Checkbox("##istrigger", &pcc.IsTrigger);
				ImGui::PopItemWidth();

			});



			DrawEntityComponents<AudioSourceComponent>("Audio Source", m_CurrentlySelectedEntity, [](AudioSourceComponent& asc) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150);

				ImGui::Text("Audio clip");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				if (ImGui::Button("load")) {
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("wav,ogg,mp3", NULL, &outPath);
					if (result == NFD_OKAY) {
						PX_CORE_TRACE("Success!");

						auto path = std::filesystem::relative(outPath, "assets/");

						Ref<AudioBuffer> buffer = AssetManager::GetAsset<AudioBuffer>(AssetManager::GetMetadata(path).Handle);
						asc.Source = Audio::CreateAudioSource(buffer);
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


				BeginDragDrop([&](AssetMetadata& metadata) {
					if (metadata.Type == AssetType::Audio) {
						asc.Source = Audio::CreateAudioSource(AssetManager::GetAsset<AudioBuffer>(metadata.Handle));
						asc.FilePath = AssetManager::GetFilePathString(metadata);
					}
					});

				ImGui::PopItemWidth();


				if (asc.Source) {
					ImGui::Separator();

					ImGui::NextColumn();
					ImGui::Text("Mixer Group Output");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);

					std::vector<const char*> Previews;
					const char* currentPreview = asc.Source->GetMixerGroup()->DebugName.c_str();

					Previews.push_back(Audio::GetGlobalMixer()->GetMasterGroup()->DebugName.c_str());
					for (int i = 0; i < Audio::GetGlobalMixer()->GetGroups().size(); i++) {
						Previews.push_back(Audio::GetGlobalMixer()->GetGroups()[i]->DebugName.c_str());
					}

					if (ImGui::BeginCombo("##mixergroupoutput", currentPreview, ImGuiComboFlags_::ImGuiComboFlags_HeightSmall)) {

						for (int i = 0; i < Previews.size(); i++) {

							bool selected = (currentPreview == Previews[i]);
							if (ImGui::Selectable(Previews[i], selected)) {
								Audio::RemoveSourceFromMixerGroup(asc.Source);

								if (i == 0) {
									asc.Source->SetMixerGroup(Audio::GetGlobalMixer()->GetMasterGroup());
								}
								else {
									asc.Source->SetMixerGroup(Audio::GetGlobalMixer()->GetGroups()[i - 1]);
								}

								Audio::AttachSourceToMixerGroup(asc.Source);
								Audio::UpdateMixerSourceGain(asc.Source->GetMixerGroup());

								break;
							}

							if(selected)
								ImGui::SetItemDefaultFocus();

						}

						ImGui::EndCombo();

					}
					
					ImGui::PopItemWidth();



					ImGui::NextColumn();
					ImGui::Text("Mute");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					bool mute = ((asc.Source->GetCurrentState() & AudioMixerStates::Mute) == AudioMixerStates::Mute);
					if (ImGui::Checkbox("##Mute", &mute)) {
						asc.Source->ShouldMute(mute);
					}
					ImGui::PopItemWidth();


					ImGui::NextColumn();
					ImGui::Text("Bypass Effects");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					bool beffects = ((asc.Source->GetCurrentState() & AudioMixerStates::Bypass) == AudioMixerStates::Bypass);
					if (ImGui::Checkbox("##BypassEffects", &beffects)) {
						asc.Source->ShouldBypassEffects(beffects);
					}
					ImGui::PopItemWidth();


					ImGui::NextColumn();
					ImGui::Text("Loop");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					bool loop = asc.Source->IsLooping();
					if (ImGui::Checkbox("##ShouldLoop", &loop)) {
						asc.Source->SetLooping(loop);
					}
					ImGui::PopItemWidth();

	

					ImGui::NextColumn();
					ImGui::Text("Play On Awake");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					bool playOnAwake = asc.Source->ShouldPlayOnAwake();
					if (ImGui::Checkbox("##PlayOnAwake", &playOnAwake)) {
						asc.Source->PlayOnAwake(playOnAwake);
					}
					ImGui::PopItemWidth();



					ImGui::NextColumn();
					ImGui::Text("Gain");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					float gain = asc.Source->GetGain();
					if (ImGui::SliderFloat("##SetGain", &gain, 0.0f, 0.5f, "%.3f")) {
						asc.Source->SetGain(gain);
					}
					ImGui::PopItemWidth();

					ImGui::NextColumn();
					ImGui::Text("Pitch");
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					float pitch = asc.Source->GetPitch();
					if (ImGui::SliderFloat("##SetPitch", &pitch, 0.0f, 1.0f, "%.3f")) {
						asc.Source->SetPitch(pitch);
					}
					ImGui::PopItemWidth();

 
//  					ImGui::NextColumn();
//  					ImGui::Text("Max Gain");
//  					ImGui::NextColumn();
//  					ImGui::PushItemWidth(-1);
//  					float maxGain = 1.0f;
// 					if (ImGui::SliderFloat("##MaxGain", &maxGain, 0.0f, 1.0f, "%.3f")) {
//  					}
//  					ImGui::PopItemWidth();
// 
// 
// 
// 					ImGui::NextColumn();
// 					ImGui::Text("Min Gain");
// 					ImGui::NextColumn();
// 					ImGui::PushItemWidth(-1);
// 					float minGain = 0.0f;
// 					if (ImGui::SliderFloat("##MinGain", &minGain, 0.0f, 1.0f, "%.3f")) {
// 					}
// 					ImGui::PopItemWidth();


					if (asc.Source->GetLowPassFilter()) {
						bool remove = false;
						ImGui::Separator();

						ImGui::NextColumn();
						if (ImGui::SmallButton("x")) {
							remove = true;
						}
						ImGui::SameLine();
						ImGui::Text("Low-pass gain");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						float lowpassGain = asc.Source->GetLowPassFilter()->GetGain();
						if (ImGui::SliderFloat("##lowpassgain", &lowpassGain, 0.0f, 1.0f, "%.3f")) {
							asc.Source->GetLowPassFilter()->SetGain(lowpassGain);
							asc.Source->ApplyFilterChanges();
						}
						ImGui::PopItemWidth();


						ImGui::NextColumn();
						ImGui::Text("Low-pass gain HF");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						float lowpassGainh = asc.Source->GetLowPassFilter()->GetHFGain();
						if (ImGui::SliderFloat("##lowpassgainh", &lowpassGainh, 0.0f, 1.0f, "%.3f")) {
							asc.Source->GetLowPassFilter()->SetHFGain(lowpassGainh);
							asc.Source->ApplyFilterChanges();
						}
						ImGui::PopItemWidth();

						if(remove)
							asc.Source->RemoveFilter(AudioFilterType::LowPass);

					}


					if (asc.Source->GetHighPassFilter()) {
						bool remove = false;
						ImGui::Separator();
						ImGui::NextColumn();
						if (ImGui::SmallButton("x")) {
							remove = true;
						}
						ImGui::SameLine();
						ImGui::Text("High-pass gain");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						float highPassGain = asc.Source->GetHighPassFilter()->GetGain();
						if (ImGui::SliderFloat("##highpassgain", &highPassGain, 0.0f, 1.0f, "%.3f")) {
							asc.Source->GetHighPassFilter()->SetGain(highPassGain);
							asc.Source->ApplyFilterChanges();
						}
						ImGui::PopItemWidth();


						ImGui::NextColumn();
						ImGui::Text("High-pass gain LF");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						float highPassGainl = asc.Source->GetHighPassFilter()->GetLFGain();
						if (ImGui::SliderFloat("##highpassgainl", &highPassGainl, 0.0f, 1.0f, "%.3f")) {
							asc.Source->GetHighPassFilter()->SetLFGain(highPassGainl);
							asc.Source->ApplyFilterChanges();
						}
						ImGui::PopItemWidth();

						if(remove)
							asc.Source->RemoveFilter(AudioFilterType::HighPass);
					}

					if (asc.Source->GetBandPassFilter()) {
						bool remove = false;

						ImGui::Separator();
						ImGui::NextColumn();
						if (ImGui::SmallButton("x")) {
							remove = true;
						}
						ImGui::SameLine();
						ImGui::Text("Bandpass Gain");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						float bandpassGain = asc.Source->GetBandPassFilter()->GetGain();
						if (ImGui::SliderFloat("##bandpassgain", &bandpassGain, 0.0f, 1.0f, "%.3f")) {
							asc.Source->GetBandPassFilter()->SetGain(bandpassGain);
							asc.Source->ApplyFilterChanges();
						}
						ImGui::PopItemWidth();


						ImGui::NextColumn();
						ImGui::Text("Bandpass gain LF");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						float bandpassgainl = asc.Source->GetBandPassFilter()->GetLFGain();
						if (ImGui::SliderFloat("##bandpassgainl", &bandpassgainl, 0.0f, 1.0f, "%.3f")) {
							asc.Source->GetBandPassFilter()->SetLFGain(bandpassgainl);
							asc.Source->ApplyFilterChanges();
						}
						ImGui::PopItemWidth();

						ImGui::NextColumn();
						ImGui::Text("Bandpass gain HF");
						ImGui::NextColumn();
						ImGui::PushItemWidth(-1);
						float bandpassgainh = asc.Source->GetBandPassFilter()->GetHFGain();
						if (ImGui::SliderFloat("##bandpassgainh", &bandpassgainh, 0.0f, 1.0f, "%.3f")) {
							asc.Source->GetBandPassFilter()->SetHFGain(bandpassgainh);
							asc.Source->ApplyFilterChanges();
						}
						ImGui::PopItemWidth();

						if(remove)
							asc.Source->RemoveFilter(AudioFilterType::BandPass);
					}


					ImGui::Separator();
					ImGui::Columns(1);

				
						if (!asc.Source->GetLowPassFilter()) {

							if (ImGui::Button("Add Low-pass")) {
								asc.Source->AddFilter(AudioFilterType::LowPass);
								ImGui::CloseCurrentPopup();
							}
							ImGui::SameLine();

						}
						if (!asc.Source->GetHighPassFilter()) {

							if (ImGui::Button("Add High-pass")) {
								asc.Source->AddFilter(AudioFilterType::HighPass);
								ImGui::CloseCurrentPopup();
							}
							ImGui::SameLine();
						}

						if (!asc.Source->GetBandPassFilter()) {

							if (ImGui::Button("Add Band-pass")) {
								asc.Source->AddFilter(AudioFilterType::BandPass);
								ImGui::CloseCurrentPopup();
							}

						}



// 					ImGui::Separator();
// 
// 					ImGui::NextColumn();
// 					ImGui::Text("High-pass gain");
// 					ImGui::NextColumn();
// 					ImGui::PushItemWidth(-1);
// 					float highPassGain = asc.Source->GetHighPassFilter()->GetGain();
// 					if (ImGui::SliderFloat("##highpassgain", &highPassGain, 0.0f, 1.0f, "%.3f")) {
// 						asc.Source->SetHighPassGain(highPassGain);
// 					}
// 					ImGui::PopItemWidth();
// 
// 
// 					ImGui::NextColumn();
// 					ImGui::Text("High-pass gain LF");
// 					ImGui::NextColumn();
// 					ImGui::PushItemWidth(-1);
// 					float highPassGainl = asc.Source->GetHighPassFilter()->GetLPGain();
// 					if (ImGui::SliderFloat("##highpassgainl", &highPassGainl, 0.0f, 1.0f, "%.3f")) {
// 						asc.Source->GetHighPassFilter()->SetGainLP(highPassGainl);
// 					}
// 					ImGui::PopItemWidth();




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
				if (!m_CurrentlySelectedEntity.HasComponent<CircleColliderComponent>()) {

					if (ImGui::Button("Circle Collider")) {

						// figure out what axis is bigger and assign its value to this variable
						float bigger = (transformScale.x > transformScale.y ? transformScale.x : transformScale.y);
						float radius = bigger / 2.0f;

						m_CurrentlySelectedEntity.AddComponent<CircleColliderComponent>(glm::vec2(0.0f, 0.0f)
							, radius, false);
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_CurrentlySelectedEntity.HasComponent<EdgeColliderComponent>()) {

					if (ImGui::Button("Edge Collider")) {
						m_CurrentlySelectedEntity.AddComponent<EdgeColliderComponent>(glm::vec2(transformPosition.x-1, transformPosition.y),
							glm::vec2(transformPosition.x+1, transformPosition.y), false);
						ImGui::CloseCurrentPopup();
					}
				}


				if (!m_CurrentlySelectedEntity.HasComponent<PolygonColliderComponent>()) {

					if (ImGui::Button("Polygon Collider")) {

						m_CurrentlySelectedEntity.AddComponent<PolygonColliderComponent>(b2Vec2{ -1, -1},
							b2Vec2{ 1, -1 },
							b2Vec2{ 0.0f, 1 }, false);
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


	void EditorSceneHierarchyPanel::RenderMaterialProperty(Ref<Material>& material, ShaderMember prop, int binding)
	{
	

		switch (prop.Type)
		{
			case ShaderBaseType::Float: 
			{

				ImGui::Text(prop.Name.c_str());
				ImGui::NextColumn();
				std::string id = std::string("##") + prop.Name;

				float temp = material->Get<float>(prop.Name, binding);
				if (ImGui::DragFloat(id.c_str(), &temp))
				{
					material->Set<float>(prop.Name, temp, binding);
					MaterialSerialization::Serialize(AssetManager::GetFilePathString(AssetManager::GetMetadata(material->Handle)), material);

				}


				ImGui::NextColumn();


				break;
			}

			case ShaderBaseType::SampledImage: 
			{
				ImGui::Text(prop.Name.c_str());
				ImGui::NextColumn();
				std::string id = std::string("##") + prop.Name;


					//if (ImGui::ImageButton((void*)spriteRect->GetHandleID(), { 128, 128 }, { 0, 1 }, { 1, 0 })) {
					//	const auto& inspector = std::dynamic_pointer_cast<EditorTextureInspector>(EditorPanelManager::Get().GetPanel("TextureInspector"));

					//	inspector->SetOpenPanel(true);
					//	inspector->SetTextureContext(spriteRect);

					//}

					//BeginDragDrop([&](AssetMetadata& metadata) {
					//	if (metadata.Type == AssetType::Texture) {
					//		spriteComp.Texture = AssetManager::GetAsset<Texture>(metadata.Handle);
					//		spriteComp.Rect = { {0, 0}, {spriteComp.Texture->GetWidth(), spriteComp.Texture->GetHeight()} };

					//	}
					//	});

					//ImGui::PopItemWidth();
					ImGui::NextColumn();
			}
		}

	}

	void EditorSceneHierarchyPanel::RenderMaterialSampledImage2D(Ref<Material>& material, SampledImage2DContainer& container, int binding)
	{
		std::filesystem::path p(container.Texture->GetFilepath());

		std::string name = p.stem().string();
		ImGui::Text(name.c_str());
		ImGui::NextColumn();
		std::string id = std::string("##") + name;


			if (ImGui::ImageButton((void*)container.Texture->GetHandleID(), { 128, 128 }, { 0, 1 }, { 1, 0 }))
			{
				const auto& inspector = std::dynamic_pointer_cast<EditorTextureInspector>(EditorPanelManager::Get().GetPanel("TextureInspector"));

				inspector->SetOpenPanel(true);
				inspector->SetTextureContext(container.Texture);
			}

			BeginDragDrop([&](AssetMetadata& metadata) {
				if (metadata.Type == AssetType::Texture) {
					container.Texture = AssetManager::GetAsset<Texture>(metadata.Handle);
					container.Rect = { {0, 0}, {container.Texture->GetWidth(), container.Texture->GetHeight()} };

				}
				});


		ImGui::NextColumn();

	}


}