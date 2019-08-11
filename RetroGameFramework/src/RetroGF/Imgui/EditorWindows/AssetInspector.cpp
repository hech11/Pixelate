#include "RGFpch.h"
#include "AssetInspector.h"

#include "RetroGF\Rendering\Sprite.h"
#include <IMGUI\imgui.h>


namespace RGF{
	void AssetInspector::Init() {

	}
	void AssetInspector::ShutDown() {

	}
	void AssetInspector::OnImguiRender() {
		ImGui::Begin("Asset Inspector");

		static bool use_clipping = false;
		ImGui::Checkbox("Clip", &use_clipping);
		if (use_clipping) {
			int desStart = 0;
			int desEnd = m_Sprites.size();
			ImGui::CalcListClipping(m_Sprites.size(), ImGui::GetTextLineHeightWithSpacing(), &desStart, &desEnd);

		}
		for (unsigned int i = 0; i < m_Sprites.size(); i++) {
			const auto& sprite = m_Sprites[i];
			std::string collaspHeader = "Sprite index : " + std::to_string(i);
			ImGui::PushID(i);

			if (ImGui::CollapsingHeader(collaspHeader.c_str())) {

				static glm::i32vec4 UV = { 0, 0, 128, 128 };

				static float maxsize = 3.4E+37f;
				ImGui::DragFloat3("Position :", &sprite->GetPosition().x, 0.01f, -maxsize, maxsize);
				ImGui::SliderInt4("Uv coords :", &UV.x, 0, 128, "%.8f");

				sprite->SetUV(UV, { 128, 128 });

			}
			ImGui::PopID();

		}

		ImGui::End();


	
	}

	void AssetInspector::PushSprite(Sprite* sprite) {
		m_Sprites.push_back(sprite);
	}


}