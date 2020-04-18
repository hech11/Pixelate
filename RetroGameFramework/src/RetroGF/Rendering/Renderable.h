#pragma once

#include "GLM/glm/glm.hpp"

#include "RetroGF/Rendering/API/Buffer.h"
#include "RetroGF/Rendering/API/VertexArray.h"

#include "RetroGF/Core/Core.h"

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

		public :
			Renderable(const glm::vec3& position, const glm::vec3& scale, const glm::vec4& color)
				: m_Position(position), m_Scale(scale), m_Color(color)
			{
				m_UV[0] = { 0.0f, 0.0f }; // -- bottom left
				m_UV[1] = { 1.0f, 0.0f }; // -- bottom right
				m_UV[2] = { 1.0f, 1.0f }; // -- top right
				m_UV[3] = { 0.0f, 1.0f }; // -- top left
				
			}

			virtual ~Renderable() {}
		public :


			inline const glm::vec3& GetPosition() const { return m_Position; }
			inline const glm::vec3& GetScale() const { return m_Scale; }
			inline const glm::vec4& GetColor () const { return m_Color; }
			inline const std::array<glm::vec2, 4>& GetUV() const { return m_UV; }

			inline glm::vec3& GetPosition()  { return m_Position; }
			inline glm::vec3& GetScale() { return m_Scale; }
			inline glm::vec4& GetColor() { return m_Color; }
			inline std::array<glm::vec2, 4> & GetUV () { return m_UV; }

			void SetPosition(const glm::vec3& pos) { m_Position = pos; }
			void SetScale(const glm::vec3& scale) { m_Scale = scale; }
			void SetColor(const glm::vec4& color) { m_Color = color; }

			void SetUV(const std::array<glm::vec2, 4>& uv) { m_UV = uv; }

		protected :
			glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
			glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };
			glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
			std::array<glm::vec2, 4> m_UV;
	};

}