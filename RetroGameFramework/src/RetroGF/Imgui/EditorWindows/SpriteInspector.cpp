#include "RGFpch.h"
#ifndef RGF_DISTRIBUTE

#include "SpriteInspector.h"

#include "RetroGF\Rendering\Sprite.h"
#include "RetroGF\Utility\Timer.h"
#include <IMGUI/imgui.h>

#include "RetroGF/Rendering/Renderer2D.h"


namespace RGF{
	void SpriteInspector::Init() {

	}
	void SpriteInspector::ShutDown() {

	}
	void SpriteInspector::OnImguiRender() {
		ImGui::Begin("Sprite Inspector");

		auto maxTWidth = Renderer2D::GetTextureManager().GetCurrentlyBound()->GetWidth();
		auto maxTHeight = Renderer2D::GetTextureManager().GetCurrentlyBound()->GetHeight();
		

		const unsigned int& spriteSize = m_Sprites.size();
		const float& textSize = ImGui::GetTextLineHeightWithSpacing();

		int desStart = 0;
		int desEnd = spriteSize;

		ImGui::CalcListClipping(spriteSize, textSize, &desStart, &desEnd);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + desStart * textSize);

		if (ImGui::CollapsingHeader("Current bound texture")) {
			ImGui::Image((ImTextureID)Renderer2D::GetTextureManager().GetCurrentlyBound()->GetHandleID(), { (float)maxTWidth, (float)maxTHeight }, { 0, 0 }, {1, -1});
		}


		for (unsigned int i = desStart; i < desEnd; i++) {
			const auto& sprite = m_Sprites[i];
			std::string collaspHeader = "Sprite index : " + std::to_string(i);

			ImGui::PushID(i);
			if (ImGui::CollapsingHeader(collaspHeader.c_str())) {
				static const float maxsize = 3.4E+33f;

				ImGui::DragFloat3(": Position", &sprite->GetPosition().x, 0.01f, -maxsize, maxsize);
				ImGui::SliderInt4(": Texture Coords", &sprite->GetTexCoords().x, 0, maxTWidth);

				auto coords = sprite->GetTexCoords();
				sprite->SetSpriteSampleCoords(coords, { 128, 128 });
			}
			ImGui::PopID();

		}

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (spriteSize - desEnd) * textSize);


		ImGui::End();


	
	}

	void SpriteInspector::PushSprite(const Ref<Sprite>& sprite) {
		m_Sprites.emplace_back(sprite);
	}


}

#endif