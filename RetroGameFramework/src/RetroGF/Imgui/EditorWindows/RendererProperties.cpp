#include "RGFpch.h"

#ifndef RGF_DISTRIBUTE

#include "RendererProperties.h"

#include <IMGUI/imgui.h>

#include <GLAD/include/glad.h>
#include <GLFW/include/GLFW/glfw3.h>

#include "RetroGF/Application.h"
#include "RetroGF/Rendering/Renderer2D.h"




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

		ImGui::Begin("Renderer Properties");

		Application& app = Application::GetApp();

		static float rgbClear[3] = { 0, 0, 0 };
		ImGui::ColorEdit3("Clear Color", rgbClear);
		app.GetRenderer().ClearColor(rgbClear[0], rgbClear[1], rgbClear[2]);

		static bool depthTest = false;
		static bool stencilTest = false;
		static bool enableBlend = false;
		static bool wireframe = false;
		if (ImGui::Checkbox("Render in wireframe", &wireframe)) {
			app.GetRenderer().RenderWireFrame(wireframe);
		}
	
		if (ImGui::Checkbox("Enable Blending", &enableBlend)) {
			app.GetRenderer().SetBlending(enableBlend);
		}
		if (enableBlend) {
			static bool openHeader = false;
			auto childsize = ((openHeader == true) ? ImVec2(300, 300) : ImVec2(300, 40));
			ImGui::BeginChild("BlendChild", childsize, true);
			if (ImGui::CollapsingHeader("Blending functions")) {
				openHeader = true;
				static int SrcCheckBox = 1;
				static int DestCheckBox = 0;
				static BlendFunc Src = BlendFunc::ONE;
				static BlendFunc Dest = BlendFunc::ZERO;

				ImguiRadioBlendFunc("ZERO",						SrcCheckBox, DestCheckBox, BlendFunc::ZERO);
				ImguiRadioBlendFunc("ONE",						SrcCheckBox, DestCheckBox, BlendFunc::ONE);
				ImGui::Spacing();
				ImguiRadioBlendFunc("SRC_COLOR",				SrcCheckBox, DestCheckBox, BlendFunc::SRC_COLOR);
				ImguiRadioBlendFunc("ONE_MINUS_SRC_COLOR",		SrcCheckBox, DestCheckBox, BlendFunc::ONE_MINUS_SRC_COLOR);
				ImguiRadioBlendFunc("DST_COLOR",				SrcCheckBox, DestCheckBox, BlendFunc::DST_COLOR);
				ImguiRadioBlendFunc("ONE_MINUS_DST_COLOR",		SrcCheckBox, DestCheckBox, BlendFunc::ONE_MINUS_DST_COLOR);
				ImguiRadioBlendFunc("SRC_ALPHA",				SrcCheckBox, DestCheckBox, BlendFunc::SRC_ALPHA);
				ImguiRadioBlendFunc("ONE_MINUS_SRC_ALPHA",		SrcCheckBox, DestCheckBox, BlendFunc::ONE_MINUS_SRC_ALPHA);
				ImguiRadioBlendFunc("DST_ALPHA",				SrcCheckBox, DestCheckBox, BlendFunc::DST_ALPHA);
				ImguiRadioBlendFunc("ONE_MINUS_DST_ALPHA",		SrcCheckBox, DestCheckBox, BlendFunc::ONE_MINUS_DST_ALPHA);
				ImguiRadioBlendFunc("CONSTANT_COLOR",			SrcCheckBox, DestCheckBox, BlendFunc::CONSTANT_COLOR);
				ImguiRadioBlendFunc("ONE_MINUS_CONSTANT_COLOR",	SrcCheckBox, DestCheckBox, BlendFunc::ONE_MINUS_CONSTANT_COLOR);
				ImguiRadioBlendFunc("CONSTANT_ALPHA",			SrcCheckBox, DestCheckBox, BlendFunc::CONSTANT_ALPHA);
				ImguiRadioBlendFunc("ONE_MINUS_CONSTANT_ALPHA",	SrcCheckBox, DestCheckBox, BlendFunc::ONE_MINUS_CONSTANT_ALPHA);
			
				for (int i = 0; i < 9; i++) {
					Src = (BlendFunc)SrcCheckBox;
					Dest = (BlendFunc)DestCheckBox;
				}
				app.GetRenderer().SetBlendFunc(Src, Dest);

			} else {
				openHeader = false;
			}
			ImGui::EndChild();
				
		}

		if (ImGui::CollapsingHeader("Tests")) {
			ImGui::BeginChild("testsChild", { 300, 100 }, true);
			if (ImGui::Checkbox("Enable Depth Test", &depthTest)) {
				app.GetRenderer().SetDepthTesting(depthTest);
			}
			if (ImGui::Checkbox("Enable Stencil Test", &stencilTest)) {
				app.GetRenderer().SetStencilTesting(stencilTest);
			}
			ImGui::EndChild();
		}
		if (ImGui::CollapsingHeader("Camera Properties")) {
			ImGui::BeginChild("CamChild", {300, 100}, true);
			ImGui::DragFloat3("Camera Pos", &app.GetCamera().GetPos().x, .01f);
			ImGui::DragFloat3("Camera Rot", &app.GetCamera().GetRot().x, .01f);
			ImGui::DragFloat("Camera angle", &app.GetCamera().GetAngle(), .01f);
			ImGui::DragFloat3("Camera Scale", &app.GetCamera().GetScale().x, .01f);
			ImGui::EndChild();
		}
		app.GetCamera().SetPosition({ app.GetCamera().GetPos().x, app.GetCamera().GetPos().y, 0.0f });


		//ImGui::ShowDemoWindow((bool*)1);
		ImGui::End();








		ImGui::Begin("Shader Manager");

		const auto& shaderIndexes = app.GetShaderManager().GetShaderIndex();
		if (!shaderIndexes.empty()) {
			for (unsigned int i = 0; i < shaderIndexes.size(); i++) {
				std::string label = "Delete###" + std::to_string(i);
				bool deleted = ImGui::Button(label.c_str());
				ImGui::SameLine();
				ImGui::Text("Shader: '%s' at location '%d'", shaderIndexes[i].first.c_str(), i);
				if (deleted)
					app.GetShaderManager().Delete(i);
			}
		} else {
			ImGui::Text("No Shaders addded to the shader manager!");

		}

		ImGui::End();




	}



}

#endif