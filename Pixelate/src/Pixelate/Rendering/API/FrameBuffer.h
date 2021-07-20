#pragma once

#include "Pixelate/Core/Core.h"
#include <Glm/glm.hpp>

namespace Pixelate {

	enum class FramebufferTextureFormat {
		None = -1,

		RGBA8,
		RED_INT,

		Depth24Stencil8,

		Depth = Depth24Stencil8
	};


	struct FramebufferTextureSpecs {
		FramebufferTextureSpecs() = default;
		FramebufferTextureSpecs(const FramebufferTextureFormat& format) : Format(format) {}

		FramebufferTextureFormat Format = FramebufferTextureFormat::None;
	};
	struct FramebufferAttachmentSpecs {
		FramebufferAttachmentSpecs() = default;
		FramebufferAttachmentSpecs(const std::initializer_list<FramebufferTextureSpecs>& attachments) : Attachments(attachments) {}

		std::vector<FramebufferTextureSpecs> Attachments;
	};

	struct FramebufferSpecs {
		FramebufferSpecs() = default;

		uint32_t Width = 0, Height = 0;
		float Scale = 1.0f;
		glm::vec4 ClearColor;

		FramebufferAttachmentSpecs Attachments;
		//uint32_t Samples = 1; // should we even support multi sampling right now?

		// TODO: blending options
		bool SwapChainTarget = false;
	};

	class Framebuffer { 
		public :

			virtual ~Framebuffer() = default;


			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void Resize(uint32_t width, uint32_t height) = 0;

			virtual void BindTexture(uint32_t attachmentID, uint32_t slot = 0) const = 0;


			virtual uint32_t GetColorAttachmentRenderID(uint32_t index) const = 0;
			virtual uint32_t GetDepthAttachmentRenderID() const = 0;
			virtual uint32_t GetRendererID() const = 0;

			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;
			virtual std::pair<uint32_t, uint32_t> GetDimensions() const = 0;



			virtual const FramebufferSpecs& GetSpecs() const = 0;

		public:
			static Ref<Framebuffer> Create(const FramebufferSpecs& specs);
	};


	class FramebufferPool {
		public :
			static void Init(uint32_t initSize = 32);
			static void Destroy();


			static void Add(const Ref<Framebuffer>& fb);
		private :
			static std::vector<Ref<Framebuffer>> m_FbPool;


	};
}