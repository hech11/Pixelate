#pragma once

#include "RetroGF/Core.h"
#include "RetroGF/Rendering/Renderable.h"

#include "RetroGF/Rendering/Camera.h"


// Renderer2D interface.

// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.


// The Renderer2D will render anything that inheriets "Renderable"
// The API equivlent contains a normal Renderer2D and BatchedRenderer2D.

// Both renderers should be implemented on CORE side.

namespace RGF {

	enum class RGF_API RenderingType {
		None = -1,
		Default,
		Batch
	};

	enum class RGF_API BlendFunc {
		None = -1,
		ZERO,
		ONE,
		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,
		DST_COLOR,
		ONE_MINUS_DST_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_COLOR,
		CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA
	};

	class RGF_API Renderer2D  {		

		public :

			virtual void SetDepthTesting(bool enable) = 0;
			virtual void SetStencilTesting(bool enable) = 0;
			virtual void RenderWireFrame(bool enable) = 0;

			virtual void SetBlending(bool enable) = 0;
			virtual void SetBlendFunc(BlendFunc& source, BlendFunc& dest) = 0;

			virtual BlendFunc& GetSrcFunc() { return m_SrcFunction; }
			virtual BlendFunc& GetDestFunc() { return m_DestFunction; }


			virtual void ClearColor(float r, float g, float b) = 0;
			virtual void ClearColor(unsigned char r, unsigned char g, unsigned char b) = 0;

			virtual void Clear() = 0;

			virtual void Submit(const Renderable* renderable) = 0;
			virtual void Render() = 0;

			virtual void Start(const RGF::Camera& camera, RGF::Shader* shader) = 0;
			virtual void End() = 0;


			virtual void Init() = 0;
			virtual void ShutDown() = 0;



			static Renderer2D* Create(const RenderingType& type = RenderingType::Batch);

		protected :
			BlendFunc m_SrcFunction, m_DestFunction;
			bool m_Blending = false;

			struct SceneData {
				Camera CurrentCamera;
				Shader* CurrentShader; // TODO: This should be replaced by a material system.
			};


			SceneData m_SceneData;
	};

}