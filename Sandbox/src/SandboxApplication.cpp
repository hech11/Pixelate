#include <RetroGF.h>

// An example application using RGF





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
		}



};


class SandboxApp : public RGF::Application {

	public :
		SandboxApp() {
			RGF_TRACE("Sandbox App was created!\n");

			PushLayer(new TestLayer());


			// Testing the filesystem.
			RGF::File TestFile("Data...data..binary data");
			RGF::FileSystem::WriteFile("TestFolder/SubFolder/test.data", TestFile);

			RGF::File ReadFile(RGF::FileSystem::ReadFile("TestFolder/SubFolder/test.data").c_str());
			RGF_MSG("%s", ReadFile.GetData());
		}
		~SandboxApp() {}

};

RGF::Application* RGF::CreateApplication() {
	return new SandboxApp;
}
