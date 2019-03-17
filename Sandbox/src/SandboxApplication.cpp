#include <RetroGF.h>

// An example application using RGF


// A test layer.
class TestLayer : public RGF::Layer {

	public :
		TestLayer() : Layer("Test Layer") {}
		~TestLayer() {}

		void Init() override { }
		void ShutDown() override { }

		void OnUpdate() override { 

			// testing the random num gen class and input polling.
			int rnd = RGF::Random::GetRandomInRange(0, 100);
			static int SameNum;
			if (SameNum == rnd) {
				RGF_WARN("\n\n\n");
				RGF_WARN("Same number!\n");
				RGF_WARN("\n\n\n");
			}
			RGF_MSG("rand nunm: %d\n", rnd);
			SameNum = rnd;

			if (RGF::InputImpl::IsKeyDown(RGF_KEY_C)) {
				RGF_MSG("C key was pressed!\n");
			}
		}
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
