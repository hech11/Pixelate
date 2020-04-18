#include <RetroGF.h>

#include "IMGUI/imgui.h"

// An example application using RGF

// A example layer.
class ExampleLayer : public RGF::Layer {

	public:
		virtual void Init() override {
		}
	
	
		virtual void ShutDown() override {
		}

		virtual void OnUpdate(float dt) override {

			{

			}

			{


			}
		}
	
	
		virtual void OnEvent(RGF::Event& e) override {
		}
	
	
		virtual void OnImguiRender() override {
		}

};


class SandboxApp : public RGF::Application {

	public :
		SandboxApp() {
			RGF_TRACE("Sandbox App was created!\n");

			PushLayer(new ExampleLayer());
		}
		~SandboxApp() {}

};

RGF::Application* RGF::CreateApplication() {
	return new SandboxApp;
}
