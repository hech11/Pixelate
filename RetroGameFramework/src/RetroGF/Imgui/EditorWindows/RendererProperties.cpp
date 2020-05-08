#include "RGFpch.h"

#ifdef RGF_USE_IMGUI


#include "RendererProperties.h"

#include <IMGUI/imgui.h>

#include <GLAD/include/glad.h>
#include <GLFW/include/GLFW/glfw3.h>

#include "RetroGF/Core/Application.h"
#include "RetroGF/Rendering/Renderer2D.h"
#include "RetroGF\Rendering\RenderCommand.h"




namespace RGF {

	void RendererProperties::Init()  {

	}
	void RendererProperties::ShutDown() {
	}

	
#define Stringify(x) #x
#define ImguiRadioBlendFunc(id, src, dest, blendid) ImGui::Text(std::string(id + std::string(" :")).c_str()); ImGui::SameLine();\
ImGui::RadioButton(std::string("Src###" + std::string(std::to_string((int)blendid))).c_str(), &src, (int)blendid);\
ImGui::SameLine();\
ImGui::RadioButton(std::string("Dest###" + std::string(std::to_string((int)blendid+15))).c_str(), &dest, (int)blendid);\





	void RendererProperties::OnImguiRender() {

		ImGui::Begin("Renderer Command");

		Application& app = Application::GetApp();

		static float rgbClear[3] = { 0, 0, 0 };
		ImGui::ColorEdit3("Clear Color", rgbClear);
		RenderCommand::SetClearColor(rgbClear[0], rgbClear[1], rgbClear[2], 1.0f);

		static bool depthTest = false;
		static bool stencilTest = false;
		static bool enableBlend = false;
		static bool wireframe = false;


		if (ImGui::Checkbox("Render in wireframe", &wireframe)) {
			RenderCommand::RenderWireFrame(wireframe);
		}
	
		if (ImGui::Checkbox("Enable Blending", &enableBlend)) {
			RenderCommand::SetBlending(wireframe);

		}
		if (enableBlend) {
			static bool openHeader = false;
			auto childsize = ((openHeader == true) ? ImVec2(300, 300) : ImVec2(300, 40));
			ImGui::BeginChild("BlendChild", childsize, true);
			if (ImGui::CollapsingHeader("Blending functions")) {
				openHeader = true;
				static int SrcCheckBox = 1;
				static int DestCheckBox = 0;
				static RendererAPI::BlendFunc Src = RendererAPI::BlendFunc::ONE;
				static RendererAPI::BlendFunc Dest = RendererAPI::BlendFunc::ZERO;

				ImguiRadioBlendFunc("ZERO",						SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::ZERO);
				ImguiRadioBlendFunc("ONE",						SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::ONE);
				ImGui::Spacing();
				ImguiRadioBlendFunc("SRC_COLOR",				SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::SRC_COLOR);
				ImguiRadioBlendFunc("ONE_MINUS_SRC_COLOR",		SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::ONE_MINUS_SRC_COLOR);
				ImguiRadioBlendFunc("DST_COLOR",				SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::DST_COLOR);
				ImguiRadioBlendFunc("ONE_MINUS_DST_COLOR",		SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::ONE_MINUS_DST_COLOR);
				ImguiRadioBlendFunc("SRC_ALPHA",				SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::SRC_ALPHA);
				ImguiRadioBlendFunc("ONE_MINUS_SRC_ALPHA",		SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::ONE_MINUS_SRC_ALPHA);
				ImguiRadioBlendFunc("DST_ALPHA",				SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::DST_ALPHA);
				ImguiRadioBlendFunc("ONE_MINUS_DST_ALPHA",		SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::ONE_MINUS_DST_ALPHA);
				ImguiRadioBlendFunc("CONSTANT_COLOR",			SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::CONSTANT_COLOR);
				ImguiRadioBlendFunc("ONE_MINUS_CONSTANT_COLOR",	SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::ONE_MINUS_CONSTANT_COLOR);
				ImguiRadioBlendFunc("CONSTANT_ALPHA",			SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::CONSTANT_ALPHA);
				ImguiRadioBlendFunc("ONE_MINUS_CONSTANT_ALPHA",	SrcCheckBox, DestCheckBox, RendererAPI::BlendFunc::ONE_MINUS_CONSTANT_ALPHA);
			
				for (int i = 0; i < 9; i++) {
					Src = (RendererAPI::BlendFunc)SrcCheckBox;
					Dest = (RendererAPI::BlendFunc)DestCheckBox;
				}
				RenderCommand::SetBlendFunc(Src, Dest);

			} else {
				openHeader = false;
			}
			ImGui::EndChild();
				
		}

		if (ImGui::CollapsingHeader("Tests")) {
			ImGui::BeginChild("testsChild", { 300, 100 }, true);
			if (ImGui::Checkbox("Enable Depth Test", &depthTest)) {
				RenderCommand::SetDepthTesting(depthTest);
			}
			if (ImGui::Checkbox("Enable Stencil Test", &stencilTest)) {
				RenderCommand::SetStencilTesting(stencilTest);
			}
			ImGui::EndChild();
		}
		if (ImGui::CollapsingHeader("Camera Properties")) {
			ImGui::BeginChild("CamChild", {300, 100}, true);

		}


		//ImGui::ShowDemoWindow((bool*)1);
		ImGui::End();


	}



}

#endif