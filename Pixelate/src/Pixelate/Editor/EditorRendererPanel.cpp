#include "PXpch.h"
#include "EditorRendererPanel.h"

#include "Pixelate/Rendering/Renderer2D.h"

#include <imgui.h>
#include "Pixelate/Asset/AssetManager.h"


namespace Pixelate
{


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


			ImGui::Text("DefaultTexturedShader");
			ImGui::SameLine();
			ImGui::PushID(0);
			if (ImGui::SmallButton("Reload"))
			{
				Renderer2D::GetShaderLibrary().Get()["DefaultTexturedShader"]->Reload();

			}
			ImGui::PopID();

			ImGui::Text("GridShader");
			ImGui::SameLine();

			ImGui::PushID(1);
			if (ImGui::SmallButton("Reload"))
			{
				Renderer2D::GetShaderLibrary().Get()["GridShader"]->Reload();
			}
			ImGui::PopID();

			int i = 2;
			for (auto& library : Renderer2D::GetShaderLibrary().Get())
			{
				ImGui::PushID(i);
				ImGui::Text(library.first.c_str());
				ImGui::SameLine();
				if (ImGui::SmallButton("Reload"))
				{
					library.second->Reload();
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