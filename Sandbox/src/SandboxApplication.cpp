#include <RetroGF.h>

#include "IMGUI/imgui.h"

#include "RetroGF/Rendering/Material.h"

// An example application using RGF

// A example layer.
class ExampleLayer : public RGF::Layer {
	RGF::Shader* shader = nullptr;
	RGF::Texture* test = nullptr;

	glm::mat4 modelMat;
	glm::vec2 pos;
	glm::vec2 pos2;

	RGF::Material* mat;
	RGF::Material* mat2;



#define LotsOfSprites 0
#define Batchrendering 0

#if LotsOfSprites

#if Batchrendering
	std::vector<RGF::BatchedSprite*> sprites;
#else
	std::vector<RGF::Sprite*> sprites;

#endif
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
			mat = new Material("Test Material", Application::GetApp().GetShaderManager().GetShaderIndex()[0].Shader);
			mat->AddUniform(new ShaderUniformInt("a", ShaderUnifromType::Int, 0));

			mat2 = new Material("Test Material2", Application::GetApp().GetShaderManager().GetShaderIndex()[0].Shader);
			mat2->AddUniform(new ShaderUniformInt("a", ShaderUnifromType::Int, 0));


			Application::GetApp().GetMaterialManager().AddMaterial(*mat);
			Application::GetApp().GetMaterialManager().AddMaterial(*mat2);


			mat->GetShader()->Bind();



#if LotsOfSprites
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

			RGF_MSG("Sprites: %d\n", sprites.size());

#else

			sprites.push_back(new Sprite({ -1.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f }, mat));
			sprites.push_back(new Sprite({ 0.0f, 0.0f, 0.0f }, { 0.2f, 0.2f, 0.2f }, { 1.0f, 1.0f, 1.0f, 1.0f }, mat2));
			

#endif


		}
		void ShutDown() override { }

		void OnUpdate(float dt) override {
			test->Bind();


			
			RGF::Application::GetApp().GetRenderer().Start(&RGF::Application::GetApp().GetCamera());

			for (unsigned int i = 0; i < sprites.size(); i++) {
				RGF::Application::GetApp().GetRenderer().Submit(sprites[i]);
			}

			RGF::Application::GetApp().GetRenderer().End();
			RGF::Application::GetApp().GetRenderer().Render();




		}
		void OnEvent(RGF::Event& e) override {}

		void OnImguiRender() override { 
			ImGui::SliderFloat2("posSprite1", &pos.x, -5.0f, 10.0f);
			ImGui::SliderFloat2("posSprite2", &pos2.x, -5.0f, 10.0f);

			

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
