#pragma once


#include "Renderable.h"
#include "RetroGF/Core.h"



// This contains classes for "Sprite" and "BatchedSprite"
/* 
	TODO: Sprites and BatchedSprites are implemented in this file. 
		  Investigate to see if it is worth seperating into their own files
		  such as "GLSprite" or "DX11Sprite".

*/

/* 
	TODO: "BatchedSprite" may be unnecessary. Investiage on combining "BatchSprite" into the "Sprite" class and 
		  switch between either batched or dynamic via a value or boolean.
*/

namespace RGF {

	class RGF_API Sprite : public Renderable {

		public :
			Sprite(glm::vec3 position, glm::vec3 scale, glm::vec4 color, Material* material) 
				: Renderable(position, scale, color), m_Material(material)
			{


				float vertex[]{
					position.x, position.y, position.z, color.x, color.y, color.z, color.w, 0.0f, 0.0f, // bottom left 0 
					position.x, position.y + scale.y, position.z, color.x, color.y, color.z, color.w, 0.0f, 1.0f, // bottom right 1
					position.x + scale.x, position.y + scale.y, position.z, color.x, color.y, color.z, color.w, 1.0f, 1.0f, // top right 2
					position.x + scale.x, position.y, position.z, color.x, color.y, color.z, color.w, 1.0f, 0.0f // top left 3
				};
				unsigned char indicies[] = {
					0, 1, 2,
					2, 3, 0
				};



				BufferLayout layout =
				{
					{BufferLayoutTypes::Float3, "aPos"},
					{BufferLayoutTypes::Float4, "aColor"},
					{BufferLayoutTypes::Float2, "aTexCoords"},
				};

				m_VertexArray = VertexArray::Create();
				m_VertexArray->Bind();

				m_VertexBuffer = VertexBuffer::Create();
				m_VertexBuffer->Bind();
				m_VertexBuffer->Resize(sizeof(vertex));





				m_VertexBuffer->SetData(vertex);
				m_VertexBuffer->SetLayout(layout);

				m_VertexArray->PushVertexBuffer(*m_VertexBuffer);

				m_IndexBuffer = IndexBuffer::Create(indicies, 6);
				m_IndexBuffer->Bind();


				
				m_VertexArray->Bind();
				m_IndexBuffer->Bind();


			}

			~Sprite() {
				delete m_VertexBuffer;
				delete m_VertexArray;
				delete m_IndexBuffer;
			}


			inline const VertexArray* GetVao() const override { return m_VertexArray; }
			inline const IndexBuffer* GetIbo() const override { return m_IndexBuffer; }
			inline Material* GetMaterial() const override { return m_Material; }

		private :
			VertexArray* m_VertexArray;
			VertexBuffer* m_VertexBuffer;
			IndexBuffer* m_IndexBuffer;
			Material* m_Material;
	};




	class RGF_API BatchedSprite : public Renderable {

		public:
			BatchedSprite(glm::vec3 position, glm::vec3 scale, glm::vec4 color)
				: Renderable(position, scale, color)
			{
			
			}


	};

}