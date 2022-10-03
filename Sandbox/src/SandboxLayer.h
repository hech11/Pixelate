#pragma once

#include <Pixelate/Core/Layer.h>

namespace Pixelate
{

	class SandboxLayer : public Layer {
		public:
			virtual void Init() override;
			virtual void ShutDown() override;

			virtual void OnUpdate(float dt) override;
			virtual void OnEvent(Pixelate::Event& e) override;

			virtual void OnImguiRender() override;
	};
}
