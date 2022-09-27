#include "PXpch.h"
#include "EditorMaterialEditorPanel.h"

#include "Pixelate/Rendering/Renderer2D.h"

#include <imgui.h>
#include "Pixelate/Asset/AssetManager.h"

namespace Pixelate
{


	void EditorMaterialEditorPanel::OnEvent(Event& e)
	{

	}


	void EditorMaterialEditorPanel::OnImguiRender()
	{
		if (!m_IsPanelOpen)
			return;

		ImGui::Begin("Material Editor", &m_IsPanelOpen);

		if (m_HasSelectedEntity)
		{
			if (m_CurrentlySelectedEntity.HasComponent<SpriteRendererComponent>())
			{
				RenderMaterialList();
				RenderShaderList();
			}
			else
			{
				ImGui::Text("Select an entity with a SpriteRendererComponent to edit it's material");
			}
		}
		else
		{
			ImGui::Text("Select an entity with a SpriteRendererComponent to edit it's material");
		}


		ImGui::End();

	}


	void EditorMaterialEditorPanel::RenderShaderList()
	{

		Ref<Material>& material = m_CurrentlySelectedEntity.GetComponent<SpriteRendererComponent>().Material;

		if (material->GetName() == "DefaultMaterial")
			return;

		ImGui::PushID(1);
		bool changedShader = false;
		std::vector<const char*> items;
		items.push_back(material->GetShader()->GetName().c_str());
		const char* current_item = items[0];


		for (auto&& [name, shader] : Renderer2D::GetShaderLibrary().Get())
		{
			if (name == items[0])
			{
				continue;
			}

			items.push_back(name.c_str());
		}
		for (auto&& [name, shader] : Renderer2D::GetShaderLibrary().GetResources())
		{
			if (name != "DefaultShader")
			{
				continue;
			}
			if (name == items[0])
			{
				continue;
			}
			items.push_back(name.c_str());
		}


		ImGui::TextDisabled(material->GetName().c_str());
		if (ImGui::BeginCombo("Shader list", current_item))
		{
			for (auto& item : items) {
				bool is_selected = (current_item == item);
				if (ImGui::Selectable(item, is_selected))
				{
					current_item = item;
					bool isAsset = false;
					bool isResource = false;

					Ref<Shader> result = nullptr;

					if (result = Renderer2D::GetShaderLibrary().Get()[current_item])
					{
						isAsset = true;
					}
					else if (result = Renderer2D::GetShaderLibrary().GetResources()[current_item])
					{
						isResource = true;
					}
					else if (!isAsset && !isResource) // Something went wrong! This item is not in our shader library for some reason...
					{
						PX_CORE_ERROR("'%s' was not in either the shader library or shader resource library!\n", current_item);
					}

					material->SetShader(result);

					MaterialSerialization::Serialize(AssetManager::GetFilePathString(AssetManager::GetMetadata(material->Handle)), material);

					changedShader = true;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}




		ImGui::Separator();

		if (!changedShader)
		{
			for (auto& table : material->GetUniformTable())
			{
				for (auto& member : table->ReflectedUniformBuffer.Members)
				{

					switch (member.Type)
					{
					case ShaderBaseType::Int32:
					{
						ImGui::Columns(2);
						ImGui::Text(member.Name.c_str());
						ImGui::NextColumn();
						int temp = material->Get<int>(member.Name, table->ReflectedUniformBuffer.Binding);
						if (ImGui::DragInt(std::string(std::string("##valuemem") + member.Name).c_str(), &temp))
						{
							material->Set(member.Name, temp, table->ReflectedUniformBuffer.Binding);
							MaterialSerialization::Serialize(AssetManager::GetFilePathString(AssetManager::GetMetadata(material->Handle)), material);

						}
						ImGui::Columns(1);

						break;

					}
					case ShaderBaseType::Float:
					{
						ImGui::Columns(2);
						ImGui::Text(member.Name.c_str());
						ImGui::NextColumn();

						float temp = material->Get<float>(member.Name, table->ReflectedUniformBuffer.Binding);
						if (ImGui::DragFloat(std::string(std::string("##valuemem") + member.Name).c_str(), &temp))
						{
							material->Set(member.Name, temp, table->ReflectedUniformBuffer.Binding);
							MaterialSerialization::Serialize(AssetManager::GetFilePathString(AssetManager::GetMetadata(material->Handle)), material);

						}
						ImGui::Columns(1);

						break;
					}
					}
				}

			}
		}
		ImGui::PopID();

	}




	void EditorMaterialEditorPanel::RenderMaterialList()
	{
		ImGui::PushID(2);

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


		if (ImGui::BeginCombo("Material list", current_item.c_str()))
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
	}

}