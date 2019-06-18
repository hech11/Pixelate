#pragma once

#include "GLM/glm/glm.hpp"

#include "RetroGF/Rendering/API/IndexBuffer.h"
#include "RetroGF/Rendering/API/VertexBuffer.h"
#include "RetroGF/Rendering/API/VertexArray.h"
#include "RetroGF/Rendering/API/Shader.h"


#include "RetroGF/Core.h"

// This is a polymorphic class that will be inherited by everything that needs to be rendered on screen.
// Such as.. sprites, text, menus, buttons... anything that needs to be rendered will be inherited by this class.

namespace RGF {

	// Data that will be sent to the GPU.
	struct RGF_API VertexData {
		glm::vec3 verticies;
		unsigned int color;
		glm::vec2 uv;
	};


	class RGF_API Renderable {
		protected :
			glm::vec3 m_Position;
			glm::vec3 m_Scale;
			glm::vec4 m_Color;
			std::array<glm::vec2, 4> m_UV;

		public :
			Renderable(const glm::vec3& position, const glm::vec3& scale, const glm::vec4& color)
				: m_Position(position), m_Scale(scale), m_Color(color)
			{
				m_UV[0] = { 0.0f, 0.0f };
				m_UV[1] = { 0.0f, 1.0f };
				m_UV[2] = { 1.0f, 1.0f };
				m_UV[3] = { 1.0f, 0.0f };
			}

			virtual ~Renderable() {}
		public :
			virtual inline const VertexArray* GetVao() const { return nullptr; }
			virtual inline const IndexBuffer* GetIbo() const { return nullptr; }
			virtual inline Shader* GetShader() const { return nullptr; };


			inline const glm::vec3& GetPosition() const { return m_Position; }
			inline const glm::vec3& GetScale() const { return m_Scale; }
			inline const glm::vec4& GetColor () const { return m_Color; }
			inline const std::array<glm::vec2, 4>& GetUV() const { return m_UV; }
	};

}