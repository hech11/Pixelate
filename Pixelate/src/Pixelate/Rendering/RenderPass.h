#pragma once

#include "Pixelate/Rendering/API/Shader/Shader.h"
#include "Pixelate/Rendering/API/FrameBuffer.h"

#include <string>

namespace Pixelate
{

	struct RenderPass
	{
		Ref<Pixelate::Framebuffer> FrameBufferTarget;
		Ref<Pixelate::Shader> Shader;
		std::string DebugName;
	};

	class RenderPassPool
	{
		public:
			void GenerateDrawList(const Ref<RenderPass> renderPass, const DrawList& list)
			{
				m_List[renderPass] = list;
			}

	
			const std::map<Ref<RenderPass>, DrawList>& GetPool() const { return m_List; }
			std::map<Ref<RenderPass>, DrawList>& GetPool() { return m_List; }


			void Destroy()
			{
				m_List.clear();
			}
		private:
			std::map<Ref<RenderPass>, DrawList> m_List;

	};
}