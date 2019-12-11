#include <RetroGF.h>

#include "IMGUI/imgui.h"

#include "RetroGF/Rendering/Material.h"
// An example application using RGF

// A example layer.
class ExampleLayer : public RGF::Layer {
	RGF::Ref<RGF::Shader> shader = nullptr;
	RGF::Ref<RGF::Texture> test = nullptr;


	RGF::Ref<RGF::Material> mat;



#define LotsOfSprites 0
	std::vector<RGF::Ref<RGF::Sprite>> sprites;


	public :

		ExampleLayer() : Layer("Test-Layer") {}
		~ExampleLayer() {}

		void Init() override {
			{
				
				RGF_PROFILE_SCOPE("Initializing assets");
				using namespace RGF;
				RGF::TextureParameters params;
				params.Filter = RGF::TextureFilter::Nearest;
				params.Format = RGF::TextureFormat::RGBA;
				params.Wrap = RGF::TextureWrap::Repeat;


				test = Texture::Create(0, 0, params);
				test->Bind();
				test->LoadTexture("assets/graphics/TestSpritesheet.png");

				Renderer2D::GetTextureManager().Add("Test texture", test);

				shader = Shader::Create();
				shader->LoadFromFile("assets/Shaders/DefaultShaderFile.shader");
				Renderer2D::GetShaderManager().Add(shader, "Shader");


				mat = std::make_shared<RGF::Material>(shader, "Test material");

				mat->AddUniforms(new UniformData<int>("u_TextureSampler", 0));
				mat->AddUniforms(new UniformData<glm::vec4>("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f }));
				mat->AddUniforms(new UniformData<int>("u_IsTexture", 1));


				Renderer2D::GetMaterialManager().AddMaterial(mat);

				mat->GetShader()->Bind();
			}

			{

				RGF_PROFILE_SCOPE("Allocating and pushing sprites");

				#if LotsOfSprites
				for (float y = -4.5f * 2; y < 4.5f * 2; y += 0.5f / 2) {
					for (float x = -8.0f * 2; x < 8.0f * 2; x += 0.5f / 2) {
						RGF::Ref<RGF::Sprite> s = std::make_shared<RGF::Sprite>
							(RGF::Sprite({ 1 - x, y, 0.0f }, { 0.5f, 0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));

						sprites.push_back(s);
						#ifndef RGF_DISTRIBUTE
						RGF::Application::GetApp().GetEngineEditor().GetAssetInspector().PushSprite(s);
						#endif
					}
				}
				#else


				RGF::Ref<RGF::Sprite> s = std::make_shared<RGF::Sprite>(RGF::Sprite({ 0.0f, -3.0f, 0.0f }, { 6.0f, 6.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));

				RGF::Ref<RGF::Sprite> s2 = std::make_shared<RGF::Sprite>(RGF::Sprite({ -1.0f, 0.0f, 0.0f }, { 6.0f, 6.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));


				sprites.push_back(s);
				sprites.push_back(s2);
				#ifndef RGF_DISTRIBUTE
				RGF::Application::GetApp().GetEngineEditor().GetAssetInspector().PushSprite(s);
				RGF::Application::GetApp().GetEngineEditor().GetAssetInspector().PushSprite(s2);
				#endif

				#endif
			}
			RGF_MSG("Sprites: %d\n", sprites.size());

		}
		void ShutDown() override { }


		void OnUpdate(float dt) override {

			{
				RGF_PROFILE_SCOPE("Test-Layer::OnUpdate::Render");
				shader->Bind();
				test->Bind();

				shader->SetUniformMatrix("u_ViewProj", RGF::Application::GetApp().GetGameviewCamera().GetViewProjectionMatrix());
				shader->SetUniform1i("u_TextureSampler", test->GetCurrentSlot());



				RGF::Renderer2D::Start(&RGF::Application::GetApp().GetGameviewCamera());


				for (const auto & sprite : sprites) {
					RGF::Renderer2D::Submit(sprite);
				}

				RGF::Renderer2D::End();
				RGF::Renderer2D::Render();
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

			PushLayer(new ExampleLayer());
		}
		~SandboxApp() {}

};

RGF::Application* RGF::CreateApplication() {
	return new SandboxApp;
}
