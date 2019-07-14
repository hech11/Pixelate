#pragma once

#include "RetroGF/Rendering/Renderable.h"
#include "RetroGF/Rendering/Renderer2D.h"


#include "RetroGF/Platform/OpenGL/GLBuffer.h"
#include "RetroGF/Platform/OpenGL/GLVertexArray.h"


#include "RGFpch.h"


// OpenGL's implementation of the "Renderer2D".

// This file contains declarations of both GLRenderer2D and GLBatchRenderer2D.

/*
	TODO: There may be no point on seperating them both into different files.
	Investigate. Maybe combine GLBatchRenderer2D into GLRenderer2D and toggle between the two via a value/boolean?
*/


namespace RGF {
	class RGF_API GLRenderer2D : public Renderer2D {

		public :

			void SetDepthTesting(bool enable) override;
			void SetStencilTesting(bool enable) override;
			void RenderWireFrame(bool enable) override;


			void SetBlending(bool enable) override;
			void SetBlendFunc(BlendFunc& source, BlendFunc& dest) override;


			void ClearColor(float r, float g, float b) override;
			void ClearColor(unsigned char r, unsigned char g, unsigned char b) override;


			void Clear() override;

			void Submit(const Renderable* renderable) override;
			void Render() override;

			void Start(const RGF::Camera& camera, RGF::Shader* shader) override;
			void End() override{}


			void Init() override{}
			void ShutDown() override{}

			
		private :
			std::deque<const Renderable*> m_RenderQueue; // TODO : This is temp. need to implement a proper render queue class prehaps?

		
	};



#define RENDERER_MAX_SPRITES 10000
#define RENDERER_VERTEX_SIZE sizeof(VertexData)
#define RENDERER_OBJECT_SIZE (RENDERER_VERTEX_SIZE  * 4)
#define RENDERER_BUFFER_SIZE (RENDERER_OBJECT_SIZE * RENDERER_MAX_SPRITES)
#define RENDERER_INDICIES_SIZE (RENDERER_MAX_SPRITES * 6)


	class RGF_API GLBatchRenderer2D : public Renderer2D {

		public:


			void SetDepthTesting(bool enable) override;
			void SetStencilTesting(bool enable) override;
			void RenderWireFrame(bool enable) override;


			void SetBlending(bool enable) override;
			void SetBlendFunc(BlendFunc& source, BlendFunc& dest) override;

			void ClearColor(float r, float g, float b) override;
			void ClearColor(unsigned char r, unsigned char g, unsigned char b) override;

			void Clear() override;

			void Submit(const Renderable* renderable) override;
			void Render() override;

			void Start(const RGF::Camera& camera, RGF::Shader* shader) override;
			void End() override;


			void Init() override;
			void ShutDown() override;

		private:
			VertexArray* m_Vao;
			VertexBuffer* m_Vbo;
			IndexBuffer* m_Ibo;


			VertexData* Buffer = nullptr;
			unsigned int m_IndexCount = 0;
	};

}

