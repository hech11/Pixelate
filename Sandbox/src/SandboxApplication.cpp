#include <RetroGF.h>

// An example application using RGF

// A example layer.
class ExampleLayer : public RGF::Layer {
	RGF::Shader* shader = nullptr;
	RGF::Texture* test = nullptr;

#define Batchrendering 0

#if Batchrendering
	std::vector<RGF::BatchedSprite*> sprites;
#else
	std::vector<RGF::Sprite*> sprites;

#endif

	public :
		ExampleLayer() : Layer("Test Layer") {}
		~ExampleLayer() {}

		void Init() override { 
		
			using namespace RGF;
			RGF::TextureParameters params;
			params.Filter = RGF::TextureFilter::Nearest;
			params.Format = RGF::TextureFormat::RGBA;
			params.Wrap = RGF::TextureWrap::Repeat;


			test = Texture::Create(0, 0, params);
			test->Bind();
			test->LoadTexture("res/graphics/sprite.png");

			shader = ShaderGenerator::GetInstance()->TexturedShader();
			shader->Bind();


			shader->SetUniform1i("u_TextureSampler", 0);



#if 1
			for (float y = 0.0f; y < 9.0f; y += 0.14f) {
				for (float x = 0.0f; x < 16.0f; x += 0.14f) {
					sprites.push_back( new
#if Batchrendering
						BatchedSprite
#else
						Sprite
#endif
						({ 1 - x, 1 - y, 0.0f }, { 0.08f, 0.08f, 1.0f }, { Random::GetRandomInRange(1.0f, 1.0f), 0.0f, 1.0f, 1.0f }
#if !Batchrendering
							, shader
#endif

							));
				}
			}

#endif

			RGF_MSG("Sprites: %d\n", sprites.size());

		}
		void ShutDown() override { }

		void OnUpdate(float dt) override {
			test->Bind();


			RGF::Application::GetApp().GetRenderer().Start(&RGF::Application::GetApp().GetCamera(), shader);
			for (unsigned int i = 0; i < sprites.size(); i++) {
//				RGF::Application::GetApp().GetRenderer().Submit(sprites[i]);
			}
			RGF::Application::GetApp().GetRenderer().End();
			RGF::Application::GetApp().GetRenderer().Render();




		}
		void OnEvent(RGF::Event& e) override {}

		void OnImguiRender() override { 
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
