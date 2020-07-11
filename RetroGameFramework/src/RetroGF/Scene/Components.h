#pragma once

#include "RetroGF/Rendering/OrthographicCamera.h"
#include "RetroGF/Rendering/API/Texture.h"
#include "glm/glm.hpp"

namespace RGF {
	

	struct NameComponent
	{
		std::string Name;

		NameComponent() = default;
		NameComponent(const NameComponent& other)
			: Name(other.Name) {}

		
	};





	// This should maybe use a mat4 in the future but just to get things working I am going to use vec3's
	struct TransformComponent {
		glm::vec3 Position, Scale;
		float Rotation;

		TransformComponent() = default;

		TransformComponent(const TransformComponent& other) 
			: Position(other.Position), Scale(other .Scale), 
			Rotation(other.Rotation)
		{ }
		TransformComponent(const glm::vec3 position, float rotation, const glm::vec3 scale)
			: Position(position), Scale(scale),
			Rotation(rotation)
		{ }

	};


	//TODO: i don't know if i agree with this design. Might refactor soon.
	struct CameraComponent {
		OrthographicCamera Camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) : Camera(other.Camera){}

	};

	struct SpriteRendererComponent {
		glm::vec4 TintColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<TextureBounds> SpriteRect;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) 
			: TintColor(other.TintColor), SpriteRect(other.SpriteRect) {}

		SpriteRendererComponent(const glm::vec4& tintColor, const Ref<TextureBounds>& spriteRect)
			: TintColor(tintColor), SpriteRect(spriteRect) {}


	};

}