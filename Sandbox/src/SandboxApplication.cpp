#include <RetroGF.h>

#include "IMGUI/imgui.h"

#include "RetroGF/Rendering/Material.h"
// An example application using RGF

// A example layer.
class ExampleLayer : public RGF::Layer {
	RGF::Ref<RGF::Shader> shader = nullptr;
	RGF::Ref<RGF::Texture> test = nullptr;


	RGF::Ref<RGF::Material> mat;



#define LotsOfSprites 1
	std::vector<RGF::Ref<RGF::Sprite>> sprites;


	public :

		ExampleLayer() : Layer("Test Layer") {}
		~ExampleLayer() {}

		void Init() override {

			using namespace RGF;
			RGF::TextureParameters params;
			params.Filter = RGF::TextureFilter::Nearest;
			params.Format = RGF::TextureFormat::RGBA;
			params.Wrap = RGF::TextureWrap::Repeat;


			test= Texture::Create(0, 0, params);
			test->Bind();
			test->LoadTexture("res/graphics/TestSpritesheet.png");

			shader = Shader::Create();
			shader->LoadFromFile("res/Shaders/DefaultShaderFile.shader");
			Renderer2D::GetShaderManager().Add(shader, "Shader");

			mat = std::make_shared<RGF::Material>(shader, "Test material");
			mat->AddUniforms(new ShaderUniformInt("u_TextureSampler", ShaderUnifromType::Int, 0));

			Renderer2D::GetMaterialManager().AddMaterial(mat);

			mat->GetShader()->Bind();


#if LotsOfSprites
			for (float y = -4.5f*2; y < 4.5f*2; y += 0.5f/2) {
				for (float x = -8.0f*2; x < 8.0f*2; x += 0.5f/2) {
					Ref<Sprite> s = std::make_shared<Sprite>
						(Sprite({ 1 - x, y, 0.0f }, { 0.5f, 0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));

					sprites.push_back(s);
					#ifndef RGF_DISTRIBUTE
					Application::GetApp().GetEngineEditor().GetAssetInspector().PushSprite(s);
					#endif
				}
			}
#else


			Ref<Sprite> s = std::make_shared<Sprite>(Sprite({ 0.0f, -3.0f, 0.0f }, { 6.0f, 6.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));
			sprites.push_back(s);
			#ifndef RGF_DISTRIBUTE
			Application::GetApp().GetEngineEditor().GetAssetInspector().PushSprite(s);
			#endif

#endif

			RGF_MSG("Sprites: %d\n", sprites.size());

		}
		void ShutDown() override { }


		void OnUpdate(float dt) override {
			
			test->Bind();
			RGF::Renderer2D::Start(&RGF::Application::GetApp().GetCamera());
			shader->SetUniformMatrix("u_ViewProj", RGF::Application::GetApp().GetCamera().GetViewProjectionMatrix());

			for (unsigned int i = 0; i < sprites.size(); i++) {
				RGF::Renderer2D::Submit(sprites[i], shader);
			}

			RGF::Renderer2D::End();
			RGF::Renderer2D::Render();


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
