#include "PXpch.h"
#include "EditorRendererPanel.h"

#include "Pixelate/Rendering/Renderer2D.h"

#include <imgui.h>
#include "Pixelate/Asset/AssetManager.h"

#include "Pixelate/Core/Application.h"
#include "Pixelate/Utility/Filesystem.h"


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

		if (ImGui::Button("Clear shader cache"))
		{
			ImGui::OpenPopup("Clearing the shader cache");
		}

		if (ImGui::BeginPopupModal("Clearing the shader cache", NULL, ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize)) {
			auto& appWindow = Application::GetApp().GetWindow();

			

			ImGui::TextColored({1.0f, 0.7f, 0.0f, 1.0f}, "Warning");
			ImGui::Text("By clearing the cache Pixelate will attempt to recompile and generate the cache.");
			ImGui::Text("If there are errors in a shader, then Pixelate cannot compile that shader.");
			ImGui::Text("This may take several seconds.");
			ImGui::Separator();

			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, { 0.35f, 0.1f, 0.1f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, { 0.5f, 0.0f, 0.0f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, { 1.0f, 0.0f, 0.2f, 1.0f });
			if (ImGui::Button("Clean", { 500/2, 50 })) { 
				FileSystem::DeleteDirectory("cache");

				auto& library = Renderer2D::GetShaderLibrary().Get();
				for (auto&& [name, entry] : library)
				{
					entry->Reload();
				}

				ImGui::CloseCurrentPopup(); 
			}
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			if (ImGui::Button("Close", { 500 / 2, 50 })) {
				ImGui::CloseCurrentPopup();
			}

			ImVec2 windowSize = ImGui::GetWindowSize();
			float x = appWindow.GetXPos() + (appWindow.GetWidth() / 2.0f) + windowSize.x / 2.0f;
			float y = appWindow.GetYPos() + (appWindow.GetHeight() / 2.0f) + windowSize.y / 2.0f;

			ImGui::SetWindowPos({ x, y });

			ImGui::SetItemDefaultFocus();
			ImGui::EndPopup();
		}



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
		bool isOpen = ImGui::TreeNodeEx("Sorting Layers");
		if (isOpen)
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
			ImGui::TreePop();
		}

	}

}