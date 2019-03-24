#pragma once


#include "Renderable.h"
#include "RetroGF/Core.h"

namespace RGF {

	class RGF_API Sprite : public Renderable {

		public :
			Sprite(glm::vec3 position, glm::vec3 scale, glm::vec4 color, Shader& shader) 
				: Renderable(position, scale, color), m_Shader(shader)
			{
				m_VertexArray = VertexArray::Create();
				float vertex[]{
					0, 0, 0, color.x, color.y, color.z, color.w,
					0, scale.y, 0, color.x, color.y, color.z, color.w,
					scale.x, scale.y, 0, color.x, color.y, color.z, color.w,
					scale.x, 0, 0, scale.x, color.y, color.z, color.w
				};


				VertexBuffer* m_VertexBuffer = VertexBuffer::Create();
				m_VertexBuffer->SetData(4 * 7 * sizeof(float), vertex);

				m_VertexArray->PushBuffer(m_VertexBuffer);


				unsigned int indicies[] = { 0, 1, 2, 2, 3 ,0 };
				m_IndexBuffer = IndexBuffer::Create(indicies, 6);

				delete m_VertexBuffer;
			}

			~Sprite() {
				delete m_VertexArray;
				delete m_IndexBuffer;
			}


			inline const VertexArray* GetVao() const { return m_VertexArray; }
			inline const IndexBuffer* GetIbo() const { return m_IndexBuffer; }
			inline Shader& GetShader() const { return m_Shader; }

		private :
			VertexArray* m_VertexArray;
			IndexBuffer* m_IndexBuffer;
			Shader& m_Shader;
	};

}