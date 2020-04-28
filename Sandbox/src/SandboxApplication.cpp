#include <RetroGF.h>

#include "IMGUI/imgui.h"
#include "GLM/glm/gtc/type_ptr.inl"

// An example application using RGF

// A example layer.
class ExampleLayer : public RGF::Layer {


	glm::vec3 SpritePosition, SpriteSize;
	glm::vec4 SpriteColor;

	RGF::Ref<RGF::Texture> LoadedFromFilepath, GeneratedTexture;


	RGF::Scoped<RGF::OrthographicCameraController> m_CameraController;
	public:
		virtual void Init() override {

			SpritePosition = { 0.0f, 0.0f, 0.0f };
			SpriteSize = { 1.0f, 1.0f, 1.0f };
			SpriteColor = { 1.00, 1.0f, 1.0f, 1.0f };

			m_CameraController = RGF::CreateScoped<RGF::OrthographicCameraController>(16.0f / 9.0f, true);

			RGF::Texture::TextureProperties props;
			props.GenerateMipMaps = false;
			LoadedFromFilepath = RGF::Texture::Create("assets/graphics/TestSpritesheet.png", props);
			GeneratedTexture = RGF::Texture::Create(1, 1);
			unsigned int data = 0xffA0Ef22;
			GeneratedTexture->SetData(&data, sizeof(unsigned int));
		}
	
	
		virtual void ShutDown() override {
		}

		virtual void OnUpdate(float dt) override {

			{

				m_CameraController->OnUpdate(dt);
			}

			{

				using namespace RGF;
				RenderCommand::Clear();
				RenderCommand::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

				Renderer2D::ResetStatistics();

				Renderer2D::Begin(&m_CameraController->GetCamera());


				Renderer2D::DrawSprite(SpritePosition, SpriteSize, SpriteColor);
				Renderer2D::DrawSprite({ 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, LoadedFromFilepath, {1.0f, 1.0f, 1.0f, 1.0f});
				Renderer2D::DrawSprite({-1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, LoadedFromFilepath, { 1.0f, 1.0f, 1.0f, 0.5f });
				Renderer2D::DrawSprite({ 2.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, GeneratedTexture, { 1.0f, 1.0f, 1.0f, 1.0f });
				Renderer2D::DrawSprite({ 2.0f, 0.0f, 0.0f }, { 3.0f, 0.5f, 1.0f }, GeneratedTexture, { 0.4f, 0.8f, 0.2f, 0.75f });

				Renderer2D::End();


			}
		}
	
	
		virtual void OnEvent(RGF::Event& e) override {
			m_CameraController->OnEvent(e);
		}
	
	
		virtual void OnImguiRender() override {
#ifndef RGF_DISTRIBUTE
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

			PushLayer(new ExampleLayer());
		}
		~SandboxApp() {}

};

RGF::Application* RGF::CreateApplication() {
	return new SandboxApp;
}
