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
				bool changedShader = false;
				Ref<Material> material = m_CurrentlySelectedEntity.GetComponent<SpriteRendererComponent>().Material;
				
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

				if (ImGui::BeginCombo("Shader list", current_item))
				{
					for (auto& item : items) {
						bool is_selected = (current_item == item);
						if (ImGui::Selectable(item, is_selected))
						{
							current_item = item;
							material->SetShader(Renderer2D::GetShaderLibrary().Get()[current_item]);

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
						for (auto& member : table.ReflectedUniformBuffer.Members)
						{

							ImGui::Text(member.Name.c_str());
							switch (member.Type)
							{
							case ShaderBaseType::Int32:
							{
								int temp = material->Get<int>(member.Name, table.ReflectedUniformBuffer.Binding);
								if (ImGui::DragInt(std::string(std::string("##valuemem") + member.Name).c_str(), &temp))
								{
									material->Set(member.Name, temp, table.ReflectedUniformBuffer.Binding);
									MaterialSerialization::Serialize(AssetManager::GetFilePathString(AssetManager::GetMetadata(material->Handle)), material);

								}
								break;

							}
							case ShaderBaseType::Float:
							{
								float temp = material->Get<float>(member.Name, table.ReflectedUniformBuffer.Binding);
								if (ImGui::DragFloat(std::string(std::string("##valuemem") + member.Name).c_str(), &temp))
								{
									material->Set(member.Name, temp, table.ReflectedUniformBuffer.Binding);
									MaterialSerialization::Serialize(AssetManager::GetFilePathString(AssetManager::GetMetadata(material->Handle)), material);

								}
								break;
							}
							}
						}

					}
				}

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

}