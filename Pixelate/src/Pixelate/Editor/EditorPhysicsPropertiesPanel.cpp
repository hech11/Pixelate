#include "PXpch.h"
#include "EditorPhysicsPropertiesPanel.h"

#include <imgui.h>
#include "glm/gtc/type_ptr.hpp"

#include <bitset>

namespace Pixelate {



	void EditorPhysicsPropertiesPanel::OnImguiRender()
	{
		if (m_IsPanelOpen)
		{

			ImGui::Begin("Physics Properties", &m_IsPanelOpen);

			RenderWorldProps();
			ImGui::Separator();
			ImGui::Text("Physics layers");
			RenderLayerMatrix();

			ImGui::End();

		}
	}

	void EditorPhysicsPropertiesPanel::RenderWorldProps()
	{
		ImGui::Text("World properties");

		auto PhysicsWorldComp = m_SceneContext->GetAllEntitiesWith<PhysicsWorldComponent>();
		for (auto entity : PhysicsWorldComp) {
			Entity e = { entity, m_SceneContext.get() };

			auto& physicsComp = e.GetComponent<PhysicsWorldComponent>();
			if (ImGui::InputFloat2("Gravity", glm::value_ptr(physicsComp.Gravity), "%.2f")) {
				physicsComp.World->SetGravity({ physicsComp.Gravity.x, physicsComp.Gravity.y });
			}
			ImGui::InputFloat("Fixed Timestep", &physicsComp.FixedTimeStep, 0.0f, 0.0f, "%.2f");
			ImGui::InputInt("Velocity Iterations", &physicsComp.VelocityIterations);
			ImGui::InputInt("Position Iterations", &physicsComp.PositionIterations);

		}
	}

	void EditorPhysicsPropertiesPanel::RenderLayerMatrix()
	{
		ImGui::BeginChild("##layersChild", {0, 0}, true);

		for (auto& layer : Physics::GetLayers()) {
			bool open = ImGui::TreeNodeEx((const void*)layer.ID, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow, layer.DebugName.c_str());
			bool deleted = false;

			ImGui::SameLine();
			if (ImGui::Button("X")) {
				deleted = true;
			}

			ImGui::SameLine();
			ImGui::TextDisabled(layer.Rule.to_string().c_str());

			if (open) {
				int position = 0;
				int childHeightMult = Physics::GetLayers().size();
				ImGui::BeginChild((std::to_string(layer.ID) + layer.DebugName).c_str(), {300, 35.0f * childHeightMult }, true);
				for (auto& subLayer : Physics::GetLayers()) {
					ImGui::Text(subLayer.DebugName.c_str());
					ImGui::SameLine();


					bool flipped = layer.Rule.test(position);
					std::string checkboxID = "##layer" + subLayer.DebugName + layer.Rule.to_string();

					if (ImGui::Checkbox(checkboxID.c_str(), &flipped)) {
						layer.Rule.set(position, flipped);
						ImGui::Separator();
					}
					position += 1;
				}
				ImGui::EndChild();
				ImGui::TreePop();
			}

			ImGui::Separator();
			if (deleted) {
				Physics::RemoveCollisionLayer(layer.ID);
			}
		}

		char buffer[255];
		memset(buffer, 0, 255);
		memcpy(buffer, m_NewLayerName.c_str(), m_NewLayerName.length());

		if (ImGui::InputText("Collision Layer Name", buffer, 255)) {
			m_NewLayerName = buffer;
		}

		if (ImGui::Button("Add Collision Layer")) {
			if(m_NewLayerName != "")
				Physics::AddCollisionLayer(m_NewLayerName);
		}

		ImGui::EndChild();

	}

}