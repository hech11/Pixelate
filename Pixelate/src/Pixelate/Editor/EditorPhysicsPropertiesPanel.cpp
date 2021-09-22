#include "PXpch.h"
#include "EditorPhysicsPropertiesPanel.h"

#include <imgui.h>
#include "glm/gtc/type_ptr.hpp"

namespace Pixelate {



	void EditorPhysicsPropertiesPanel::OnImguiRender()
	{
		if (m_IsPanelOpen)
		{

			ImGui::Begin("Physics Properties", &m_IsPanelOpen);


			auto PhysicsWorldComp = m_SceneContext->GetAllEntitiesWith<PhysicsWorldComponent>();
			for (auto s : PhysicsWorldComp) {
				Entity e = { s, m_SceneContext.get() };

				auto& physicsComp = e.GetComponent<PhysicsWorldComponent>();
				if (ImGui::InputFloat2("Gravity", glm::value_ptr(physicsComp.Gravity), "%.2f")) {
					physicsComp.World->SetGravity({ physicsComp.Gravity.x, physicsComp.Gravity.y });
				}
				ImGui::InputFloat("Fixed Timestep", &physicsComp.FixedTimeStep, 0.0f, 0.0f, "%.2f");
				ImGui::InputInt("Velocity Iterations", &physicsComp.VelocityIterations);
				ImGui::InputInt("Position Iterations", &physicsComp.PositionIterations);

				ImGui::BeginChild("##filters");
				for (auto& [id, name] : Physics::GetFilters()) {

					
					std::string tree = name + "--" + std::to_string(id).c_str();
					bool open = ImGui::TreeNodeEx(tree.c_str());
					bool deleted = false;
					ImGui::SameLine();
					if (ImGui::SmallButton("X")) {
						deleted = true;
					}
					if (open) {
						int16_t rule = 0;
						for (auto& [filterID, filterRule] : Physics::GetFilterRules()) {

							ImGui::Text(Physics::ConvertFilterIDToString(filterID).c_str());
							ImGui::SameLine();


							// idk if this causes confliction within the filter rules or its just imgui id stuff.
							bool value = filterID & filterRule;
							std::string checkboxID = "##collide" + std::to_string(filterID) + std::to_string(filterRule) + tree;
							if(ImGui::Checkbox(checkboxID.c_str(), &value)) {
								if (value)
									filterRule = Physics::FlipFilter(filterID, filterRule);
								else
									filterRule = Physics::UnflipFilter(filterID, filterRule);

							}

							ImGui::Separator();
							rule++;
						}
						ImGui::TreePop();

					}

					if(deleted)
						Physics::RemoveCollisionFilter(name);
				}

				ImGui::Separator();
				char buffer[255];
				memset(buffer, 0, 255);
				memcpy(buffer, m_NewFilterName.c_str(), m_NewFilterName.length());
				if (ImGui::InputText("Collision Layer Name", buffer, 255)) {
					m_NewFilterName = buffer;
				}
				if (ImGui::Button("Add Collision Filter")) {
					Physics::AddCollisionFilter(m_NewFilterName);
				}
				ImGui::EndChild();


			}

			ImGui::End();

		}
	}

}