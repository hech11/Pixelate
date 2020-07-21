#pragma once

#include "RetroGF/Rendering/OrthographicCamera.h"
#include "RetroGF/Rendering/API/Texture.h"
#include "glm/glm.hpp"
#include "RetroGF/Core/AABB.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"



namespace RGF {
	

	struct NameComponent
	{
		std::string Name;

		NameComponent() = default;
		NameComponent(const NameComponent& other)
			: Name(other.Name) {}
		NameComponent(const std::string& name)
			: Name(name) {}


		
	};


	struct TransformComponent {
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;

		TransformComponent(const TransformComponent& other) :
			Transform(other.Transform)
		{ 
		}
		TransformComponent(const glm::mat4& transform)
			: Transform(transform)
		{
		}

		std::tuple<glm::vec3, glm::quat, glm::vec3> DecomposeTransform() {
			glm::vec3 position, scale, skew;
			glm::quat qua;
			glm::vec4 perspective;
			glm::decompose(Transform, scale, qua, position, skew, perspective);

			return { position, qua, scale };
		}

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