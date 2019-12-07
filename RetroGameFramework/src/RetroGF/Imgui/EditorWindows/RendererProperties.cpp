#include "RGFpch.h"

#ifndef RGF_DISTRIBUTE

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
			ImGui::DragFloat3("Camera Pos", &app.GetGameviewCamera().GetPos().x, .01f);
			ImGui::DragFloat("Camera Rot", &app.GetGameviewCamera().GetRot(), .01f);
			ImGui::DragFloat("Camera angle", &app.GetGameviewCamera().GetAngle(), .01f);
			ImGui::DragFloat3("Camera Scale", &app.GetGameviewCamera().GetScale().x, .01f);
			ImGui::EndChild();
		}
		app.GetGameviewCamera().SetPosition({ app.GetGameviewCamera().GetPos().x, app.GetGameviewCamera().GetPos().y, 0.0f });


		//ImGui::ShowDemoWindow((bool*)1);
		ImGui::End();



		ImGui::Begin("Material Manager");

		auto& materialIndexes = Renderer2D::GetMaterialManager().GetMaterialList();
		const auto& shaderIndexes = Renderer2D::GetShaderManager().GetShaderIndex();

		
		for (unsigned int i = 0; i < materialIndexes.size(); i++) {

			std::string collaspHeader = "Material: " + materialIndexes[i]->GetName();
			if (ImGui::CollapsingHeader(collaspHeader.c_str())) {


				if (!shaderIndexes.empty()) {
					for (unsigned int i = 0; i < shaderIndexes.size(); i++) {
						ImGui::Text("Shader: '%s' at location '%d'", shaderIndexes[i].Name.c_str(), i);
					}
				} else {
					ImGui::Text("No Shaders addded to the shader manager!");
				}



				auto& currentMatUniforms = materialIndexes[i]->GetUniforms();
				auto& currentMat = materialIndexes[i];


				for (unsigned int j = 0; j < currentMatUniforms.size(); j++) {
					ImGui::Text("Uniform : '%s' (location: %d)", currentMatUniforms[j]->Name.c_str(), currentMatUniforms[j]->Location);

					std::string id = std::to_string(i) + std::to_string(j);

					const auto& mat = currentMat;
					const auto& uniform = currentMatUniforms[j];
					switch (currentMatUniforms[j]->Type) {


						case ShaderUnifromType::Int:
							ImGui::SliderInt(id.c_str(), (int*)uniform->ReturnData(), 0, 100);
							break;


						case ShaderUnifromType::Int2:
						{
							auto& value = *(glm::i32vec2*)uniform->ReturnData();
							ImGui::SliderInt2(id.c_str(), &value.x, 0, 100);
							break;

						}


						case ShaderUnifromType::Int3:
						{
							auto& value = *(glm::i32vec3*)uniform->ReturnData();
							ImGui::SliderInt3(id.c_str(), &value.x, 0, 100);
							break;

						}



						case ShaderUnifromType::Int4:
						{
							auto& value = *(glm::i32vec4*)uniform->ReturnData();
							ImGui::SliderInt4(id.c_str(), &value.x, 0, 100);
							break;
						}

						case ShaderUnifromType::Float:
						{
							auto& value = *(float*)uniform->ReturnData();
							ImGui::SliderFloat(id.c_str(), (float*)uniform->ReturnData(), -100, 100);
							break;
						}

						case ShaderUnifromType::Float2:
						{
							auto& value = *(glm::vec2*)uniform->ReturnData();
							ImGui::SliderFloat2(id.c_str(), &value.x, -100, 100);
							break;
						}



						case ShaderUnifromType::Float3:
						{
							auto& value = *(glm::vec3*)uniform->ReturnData();
							ImGui::SliderFloat3(id.c_str(), &value.x, -100, 100);
							break;
						}



						case ShaderUnifromType::Float4:
						{
							auto& value = *(glm::vec4*)uniform->ReturnData();
							ImGui::SliderFloat4(id.c_str(), &value.x, -100, 100);
							break;
						}


						case ShaderUnifromType::Mat4:
							break;


					}

					Renderer2D::GetMaterialManager().SumbitUniforms(currentMat, currentMatUniforms[j]);
					ImGui::Separator();
				}
			
			
			}
		}

		ImGui::End();





	}



}

#endif