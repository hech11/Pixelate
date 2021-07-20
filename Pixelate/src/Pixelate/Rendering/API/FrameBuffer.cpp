#include "PXpch.h"
#include "Framebuffer.h"

#include "Pixelate/Rendering/RendererAPI.h"
#include "Pixelate/Platform/OpenGL/GLFramebuffer.h"


namespace Pixelate {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecs& specs) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::OpenGL:
				return CreateRef<GLFramebuffer>(specs);
		}
	}



	std::vector<Pixelate::Ref<Pixelate::Framebuffer>> FramebufferPool::m_FbPool;
	void FramebufferPool::Init(uint32_t initSize)
	{
		m_FbPool.reserve(initSize);
	}

	void FramebufferPool::Destroy()
	{
		m_FbPool.clear();
	}

	void FramebufferPool::Add(const Ref<Framebuffer>& fb)
	{
		m_FbPool.emplace_back(fb);
	}


}