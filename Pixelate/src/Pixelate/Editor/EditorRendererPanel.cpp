#include "PXpch.h"
#include "EditorRendererPanel.h"

#include "Pixelate/Rendering/Renderer2D.h"

#include <imgui.h>
#include "Pixelate/Asset/AssetManager.h"


namespace Pixelate
{

	static std::string ShaderBaseTypeToStr(ShaderBaseType& type)
	{
		switch (type)
		{
			case ShaderBaseType::Bool:			return "bool";
			case ShaderBaseType::Float:			return "float";
			case ShaderBaseType::Int8:			return "int8";
			case ShaderBaseType::Int16:			return "int16";
			case ShaderBaseType::Int32:			return "int32";
			case ShaderBaseType::Mat4:			return "mat4";
			case ShaderBaseType::SampledImage:	return "sampled image";
		}
	}
	void EditorRendererPanel::OnImguiRender()
	{

		if (!m_IsPanelOpen)
			return;

		ImGui::Begin("Renderer", &m_IsPanelOpen);

		RenderStats();
		RenderSettings();
		RenderShaderProperties();
		RenderLayers();
		ImGui::End();

	}

	

	void EditorRendererPanel::RenderStats()
	{
		bool isStatsOpen = ImGui::TreeNodeEx("Stats");


		if (isStatsOpen)
		{
			

			ImGui::TreePop();
		}
	}

	void EditorRendererPanel::RenderSettings()
	{
		bool isSettingsOpen = ImGui::TreeNodeEx("Settings");
		if (isSettingsOpen)
		{
			ImGui::TreePop();
		}

	}

	void EditorRendererPanel::RenderShaderProperties()
	{
		bool isPropsOpen = ImGui::TreeNode("Shader Properties");
		if (isPropsOpen)
		{

			int i = 0;
			for (auto&& [name, shader] : Renderer2D::GetShaderLibrary().Get())
			{
				ImGui::PushID(i);
				bool openShaderNode = ImGui::TreeNode(name.c_str());
				
				ImGui::SameLine();
				if (ImGui::SmallButton("Reload"))
				{
					shader->Reload();
				}

				if (openShaderNode)
				{
					for (auto& resource : shader->GetResources())
					{

						ImGui::Text("%s", resource.Name.c_str());
						ImGui::Text("Sampled buffer size: %d", resource.UniformBufferSize);
						ImGui::Text("Sampled image size: %d", resource.SampledBufferSize);
						ImGui::NewLine();
						for (auto& ubo : resource.Uniforms)
						{
							ImGui::Text("Binding: %d", ubo.Binding);
							ImGui::Text("Struct size: %d", ubo.StructSize);
							ImGui::Text("Member size: %d", ubo.MemberSize);
							ImGui::NewLine();

							for (auto& member : ubo.Members)
							{
								ImGui::Text("%s", member.Name.c_str());
								ImGui::Text("Type %s", ShaderBaseTypeToStr(member.Type).c_str());
								ImGui::Text("Offset %d", member.Offset);
							}
							ImGui::NewLine();

						}
						ImGui::NewLine();

					}
					ImGui::TreePop();
				}

				ImGui::PopID();
				i++;

			}

			ImGui::TreePop();

		}
	}


	void EditorRendererPanel::RenderLayers()
	{

		ImGui::BeginChild("##sortinglayerschild", { 0, 0 }, true);

		for (auto& layer : SortingLayerManager::GetLayers()) {
			bool deleted = false;


			int childHeightMult = SortingLayerManager::GetLayers().size();
			ImGui::Text(layer.second.c_str());
			ImGui::SameLine();
			ImGui::TextDisabled(std::to_string(layer.first).c_str());
			ImGui::SameLine();

			ImGui::PushID(layer.first);
			if (layer.first != 0 && ImGui::SmallButton("X")) {
				auto spriteView = m_SceneContext->GetReg().view<SpriteRendererComponent>();
				for (auto& entity : spriteView)
				{
					Entity e = { entity, m_SceneContext.get() };
					auto& src = e.GetComponent<SpriteRendererComponent>();

					if (src.SortingLayer == layer.first)
					{
						src.SortingLayer = layer.first - 1;
						if (src.SortingLayer < 0)
							src.SortingLayer = 0;
					}

				}
				PX_CORE_WARN("Pressed\n");
				SortingLayerManager::RemoveLayer(layer.first);

				m_SceneContext->GetReg().sort<SpriteRendererComponent>([](const SpriteRendererComponent& lhs, const SpriteRendererComponent& rhs)
					{
						return lhs.RenderOrder + (1 + lhs.SortingLayer * 100) < rhs.RenderOrder + (1 + rhs.SortingLayer * 100);
					});
			}
			else
				ImGui::NewLine();
			ImGui::PopID();
			ImGui::Separator();
		}



		// Limit the amount of layers we can add.
		if (SortingLayerManager::GetMaxNumberOfLayers() > SortingLayerManager::GetLayers().size())
		{
			char buffer[255];
			memset(buffer, 0, 255);
			memcpy(buffer, m_NewLayerName.c_str(), m_NewLayerName.length());

			if (ImGui::InputText("Layer Name", buffer, 255)) {
				m_NewLayerName = buffer;
			}

			if (ImGui::Button("Add Layer")) {
				if (m_NewLayerName != "")
					SortingLayerManager::AddLayer(m_NewLayerName);
			}
		}

		ImGui::EndChild();


	}

}