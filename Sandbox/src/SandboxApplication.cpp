#include <RetroGF.h>




// An example application using RGF
#include "RetroGF/Imgui/ImguiLayer.h"
#include "../../IMGUI/imgui.h"



// A test layer.
class TestLayer : public RGF::Layer {

	public :
		TestLayer() : Layer("Test Layer") {}
		~TestLayer() {}

		void Init() override { }
		void ShutDown() override { }

		void OnUpdate() override { 	}
		void OnEvent(RGF::Event& e) override {}

		void OnImguiRender() override { 
			ImGui::Begin("Testing window!");

			ImGui::End();
		}



};


class SandboxApp : public RGF::Application {

	public :
		SandboxApp() {
			RGF_TRACE("Sandbox App was created!\n");

			PushLayer(new TestLayer());
		}
		~SandboxApp() {}

};

RGF::Application* RGF::CreateApplication() {
	return new SandboxApp;
}
