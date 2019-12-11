#include "RGFpch.h"
#include "Sprite.h"

#include "RetroGF/Core/Application.h"


namespace RGF {


	Sprite::Sprite(glm::vec3 position, glm::vec3 scale, glm::vec4 color)
		: Renderable(position, scale, color), m_TexCoords({0, 0, 128, 128})
	{
	}



	void Sprite::SetSpriteSampleCoords(const glm::i32vec4& UVTexCoords, const glm::vec2& textureDimensions) {
		m_TexCoords = UVTexCoords;

		const float xo = (UVTexCoords.x / textureDimensions.x);
		const float yo = (UVTexCoords.y / textureDimensions.y);

		const float uo = (UVTexCoords.z / textureDimensions.x);
		const float vo = (UVTexCoords.w / textureDimensions.y);

		const float xu = xo + uo;
		const float yv = yo + vo;


		m_UV[0] = { xo, 1.0f - yv }; // -- bottom left
		m_UV[1] = { xu, 1.0f - yv }; // -- bottom right
		m_UV[2] = { xu, 1.0f - yo }; // -- top right
		m_UV[3] = { xo, 1.0f - yo }; // -- top left
	}

}