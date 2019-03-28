#pragma once

#include "RetroGF/Rendering/Renderer2D.h"
#include "GLVertexBuffer.h"
#include "GLVertexArray.h"
#include "GLIndexBuffer.h"


#define RENDERER_MAX_SPRITES 10000
#define RENDERER_VERTEX_SIZE sizeof(VertexData)
#define RENDERER_OBJECT_SIZE (RENDERER_VERTEX_SIZE  * 4)
#define RENDERER_BUFFER_SIZE (RENDERER_OBJECT_SIZE * RENDERER_MAX_SPRITES)
#define RENDERER_INDICIES_SIZE (RENDERER_MAX_SPRITES * 6)


namespace RGF {
	class RGF_API GLBatchRenderer2D : public Renderer2D {

		public :

			void Clear() override;

			void Submit(const Renderable* renderable) override;
			void Render() override;

			void Start() override;
			void End() override;


			void Init() override;
			void ShutDown() override;

		private :
			GLVertexArray* m_Vao;
			GLVertexBuffer* m_Vbo;
			GLIndexBuffer* m_Ibo;
			VertexData* Buffer;
			unsigned int m_IndexCount = 0;
	};
}