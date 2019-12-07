#include "RGFpch.h"
#ifndef RGF_DISTRIBUTE

#include "AssetInspector.h"

#include "RetroGF\Rendering\Sprite.h"
#include "RetroGF\Utility\Timer.h"
#include <IMGUI\imgui.h>


namespace RGF{
	void AssetInspector::Init() {

	}
	void AssetInspector::ShutDown() {

	}
	void AssetInspector::OnImguiRender() {
		ImGui::Begin("Asset Inspector");


		const unsigned int& spriteSize = m_Sprites.size();
		const float& textSize = ImGui::GetTextLineHeightWithSpacing();

		int desStart = 0;
		int desEnd = spriteSize;

		ImGui::CalcListClipping(spriteSize, textSize, &desStart, &desEnd);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + desStart * textSize);


		for (unsigned int i = desStart; i < desEnd; i++) {
			const auto& sprite = m_Sprites[i];
			std::string collaspHeader = "Sprite index : " + std::to_string(i);

			if (ImGui::CollapsingHeader(collaspHeader.c_str())) {
			ImGui::PushID(i);
				static int tex[4] = { 0, 0, 128, 128 };
				static const float maxsize = 3.4E+33f;

				ImGui::DragFloat3(": Position", &sprite->GetPosition().x, 0.01f, -maxsize, maxsize);
				ImGui::SliderInt4(": Texture Coords", tex, 0, 128);

				sprite->SetUV({ tex[0], tex[1], tex[2], tex[3] }, { 128, 128 });
			ImGui::PopID();

			}

		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (spriteSize - desEnd) * textSize);


		ImGui::End();


	
	}

	void AssetInspector::PushSprite(const Ref<Sprite>& sprite) {
		m_Sprites.emplace_back(sprite);
	}


}

#endif