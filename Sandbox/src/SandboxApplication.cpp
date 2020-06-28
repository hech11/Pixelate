#include <RetroGF.h>

#include "IMGUI/imgui.h"
#include "GLM/glm/gtc/type_ptr.inl"


// An example application using RGF

// A example layer.
class ExampleLayer : public RGF::Layer {


	glm::vec3 SpritePosition, SpriteSize;
	float Rotation = 0.0f;
	glm::vec4 SpriteColor;

	RGF::Ref<RGF::Texture> LoadedFromFilepath, GeneratedTexture;
	RGF::Ref<RGF::TextureBounds> SmileySprite, GaspSprite;

	RGF::ParticleSystem particleSystem;
	RGF::ParticleProperties particleProps;

	RGF::Scoped<RGF::OrthographicCameraController> m_CameraController;
	RGF::Ref<RGF::AudioSource> toneSFX, sameToneSfx;

	public:
		virtual void Init() override {

			SpritePosition = { 0.0f, 0.0f, 0.0f };
			SpriteSize = { 1.0f, 1.0f, 1.0f };
			SpriteColor = { 1.00, 1.0f, 1.0f, 1.0f };

			m_CameraController = RGF::CreateScoped<RGF::OrthographicCameraController>(16.0f / 9.0f, true);

			LoadedFromFilepath = RGF::Texture::Create("assets/graphics/TestSpritesheet.png");
			GeneratedTexture = RGF::Texture::Create(1, 1);
			unsigned int data = 0xffA0Ef22;
			GeneratedTexture->SetData(&data, sizeof(unsigned int));

			SmileySprite = RGF::TextureBounds::Create(LoadedFromFilepath, { 0, 0, 16 ,16 });
			GaspSprite = RGF::TextureBounds::Create(LoadedFromFilepath, { 16, 0, 16 ,16 });

			particleProps.VelocityVariation1 = { -2.0f, -2.0f };
			particleProps.VelocityVariation2 = { 2.0f, 2.0f };
			particleProps.LifeTime = 1.0f;
			particleProps.SizeBegin= 1.5f;
			particleProps.SizeVariation = 0.0f;
			particleProps.SizeEnd = 0.0f;
			particleProps.ColorBegin = { 1.0f, 1.0f, 1.0f, 1.0f };
			particleProps.ColorEnd = { 0.0f, 0.0f, 1.0f , 0.0f };

			toneSFX = RGF::Audio::CreateAudioSource("assets/audio/tone-ogg.ogg");
			sameToneSfx = RGF::Audio::CreateAudioSource("assets/audio/tone-ogg.ogg");
		}
	
	
		virtual void ShutDown() override {
		}

		virtual void OnUpdate(float dt) override {

			using namespace RGF;

			{

				m_CameraController->OnUpdate(dt);


				if (RGF::Input::IsMouseButtonDown(0)) {
					float x = RGF::Input::GetMousePosX();
					float y = RGF::Input::GetMousePosY();
					float width = RGF::Application::GetApp().GetWindow().GetWidth();
					float height = RGF::Application::GetApp().GetWindow().GetHeight();

					auto bounds = m_CameraController->GetBounds();
					auto pos = m_CameraController->GetCamera().GetPos();
					x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
					y = bounds.GetHeight() * 0.5f - (y/height)*bounds.GetHeight();
					particleProps.Position = { x + pos.x, y + pos.y };

					for (unsigned int i = 0; i < particleSystem.InitData.SpawnRate; i++) {
						particleSystem.Emit(particleProps);
					}

				}

				particleSystem.OnUpdate(dt);
				Audio::Update();


			}

			{

				RenderCommand::Clear();
				RenderCommand::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

				Renderer2D::ResetStatistics();

				Renderer2D::BeginScene(&m_CameraController->GetCamera());

				
				Renderer2D::DrawSprite(SpritePosition, Rotation, SpriteSize, SpriteColor);
				Renderer2D::DrawSprite({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, {.5f, .5f, .5f, 1.0f});
				Renderer2D::DrawSprite({ 2.0f, 1.0f, 1.0f }, 50.0f,{.5f, 5.f, 1.0f}, SpriteColor);
				Renderer2D::DrawSprite({ 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, LoadedFromFilepath, { 1.0f, 1.0f, 1.0f, 1.0f });
				Renderer2D::DrawSprite({ 0.0f, -2.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, SmileySprite, { 1.0f, 1.0f, 1.0f, 1.0f });
				Renderer2D::DrawSprite({ 1.0f, -2.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, GaspSprite, { 0.5f, 1.0f, 1.0f, 0.5f });
				Renderer2D::DrawSprite({ -1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, LoadedFromFilepath, { 1.0f, 1.0f, 1.0f, 0.5f });
				Renderer2D::DrawSprite({ -2.0f, 1.0f, 0.0f }, 25.0f, { 1.0f, 1.0f, 1.0f }, LoadedFromFilepath, { 1.0f, 1.0f, 1.0f, 0.5f });
				Renderer2D::DrawSprite({ 2.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, GeneratedTexture, { 1.0f, 1.0f, 1.0f, 1.0f });
				Renderer2D::DrawSprite({ 2.0f, 0.0f, 0.0f }, { 3.0f, 0.5f, 1.0f }, GeneratedTexture, { 0.4f, 0.8f, 0.2f, 0.75f });
				Renderer2D::DrawSprite({ 2.0f, 2.0f, 0.0f }, 75.0f, { 1.0f, 1.0f, 1.0f }, GeneratedTexture, { 1.0f, 1.0f, 1.0f, 1.0f });
				

				particleSystem.OnRender();
				Renderer2D::EndScene();



			}
		}
	
		bool OnKeyPressedEvent(RGF::KeyPressedEvent& e) {
			if (e.GetKeyCode() == RGF_KEY_P && e.GetRepeatCount() == 0) {
				toneSFX->Play();
				return true;
			}

			if (e.GetKeyCode() == RGF_KEY_O && e.GetRepeatCount() == 0) {
				sameToneSfx->Play();
				return true;
			}

			if (e.GetKeyCode() == RGF_KEY_S && e.GetRepeatCount() == 0) {
				toneSFX->Stop();
				return true;

			}
			if (e.GetKeyCode() == RGF_KEY_I && e.GetRepeatCount() == 0) {
				toneSFX->Pause();
				return true;

			}

			if (e.GetKeyCode() == RGF_KEY_L && e.GetRepeatCount() == 0) {
				static bool loop = false;
				loop = !loop;
				RGF_TRACE("Looping is %d\n", loop);
				toneSFX->SetLooping(loop);
				return true;

			}


			return false;
		}
	
		virtual void OnEvent(RGF::Event& e) override {
			m_CameraController->OnEvent(e);
			RGF::EventDispatcher dispatcher(e);

			dispatcher.Dispatch<RGF::KeyPressedEvent>(std::bind(&ExampleLayer::OnKeyPressedEvent, this, std::placeholders::_1));

		}
	
	
		virtual void OnImguiRender() override {
#ifdef RGF_USE_IMGUI
			using namespace RGF;

			ImGui::Begin("Renderer stats");
			ImGui::Text("DrawCalls: %d", Renderer2D::GetStats().DrawCalls);
			ImGui::Text("Max Sprites: %d", Renderer2D::GetStats().MaxSprites);
			ImGui::Text("Max VBO size: %d", Renderer2D::GetStats().MaxVertexBufferSize);
			ImGui::Text("Max IBO size: %d", Renderer2D::GetStats().MaxIndexBuferSize);
			ImGui::Text("Vertex ptr size: %d", Renderer2D::GetStats().VertexSize);
			ImGui::Text("IndexCount: %d", Renderer2D::GetStats().IndexCount);
			ImGui::End();

			ImGui::Begin("Sprite props");
			ImGui::SliderFloat3("Sprite Position", glm::value_ptr(SpritePosition), -10.0f, 10.0f, "%.2f");
			ImGui::SliderFloat("Sprite Rotation", &Rotation, -360.0f, 360.0f, "%.2f");
			ImGui::SliderFloat3("Sprite Size", glm::value_ptr(SpriteSize), -10.0f, 10.0f, "%.2f");
			ImGui::ColorPicker4("Sprite Color", glm::value_ptr(SpriteColor));
			ImGui::End();


#endif


		}

};


class SandboxApp : public RGF::Application {

	public :
		SandboxApp() {
			RGF_TRACE("Sandbox App was created!\n");

			//PushLayer(new ExampleLayer());
		}
		~SandboxApp() {}

};

RGF::Application* RGF::CreateApplication() {
	return new SandboxApp;
}
