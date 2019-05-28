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
	};


	class RGF_API Renderable {
		protected :
			glm::vec3 m_Position;
			glm::vec3 m_Scale;
			glm::vec4 m_Color;

		public :
			Renderable(const glm::vec3& position, const glm::vec3& scale, const glm::vec4& color)
				: m_Position(position), m_Scale(scale), m_Color(color)
			{}

			virtual ~Renderable() {}
		public :
			virtual inline const VertexArray* GetVao() const { return nullptr; }
			virtual inline const IndexBuffer* GetIbo() const { return nullptr; }
			virtual inline Shader* GetShader() const { return nullptr; };


			inline const glm::vec3& GetPosition() const { return m_Position; }
			inline const glm::vec3& GetScale() const { return m_Scale; }
			inline const glm::vec4& GetColor () const { return m_Color; }
	};

}