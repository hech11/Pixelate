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
#define ImguiRadioBlendFunc(blendfunc, x) if (ImGui::RadioButton(Stringify(blendfunc), x == blendfunc)) { x = blendfunc; }
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

		if (ImGui::Checkbox("Enable Depth Test", &depthTest)) {
			app.GetRenderer().SetDepthTesting(depthTest);
		}
		if(ImGui::Checkbox("Enable Stencil Test", &stencilTest)) {
			app.GetRenderer().SetStencilTesting(stencilTest);
		}
		if (ImGui::Checkbox("Render in wireframe", &wireframe)) {
			app.GetRenderer().RenderWireFrame(wireframe);
		}
		if (ImGui::Checkbox("Enable Blending", &enableBlend)) {
			app.GetRenderer().SetBlend(enableBlend);
		}
		if (enableBlend) {
			static bool SourceCheckBox[9];
			static bool DestCheckBox[9];
			static BlendFunc Src;
			static BlendFunc Dest;
			ImGui::Text("Source");

			ImguiRadioBlendFunc(BlendFunc::ZERO, Src);
			ImguiRadioBlendFunc(BlendFunc::ONE, Src);
			ImguiRadioBlendFunc(BlendFunc::ONE_MINUS_SRC_COLOR, Src);
			ImguiRadioBlendFunc(BlendFunc::DST_COLOR, Src);
			ImguiRadioBlendFunc(BlendFunc::ONE_MINUS_DST_COLOR, Src);
			ImguiRadioBlendFunc(BlendFunc::CONSTANT_COLOR, Src);
			ImguiRadioBlendFunc(BlendFunc::ONE_MINUS_CONSTANT_COLOR, Src);
			ImguiRadioBlendFunc(BlendFunc::CONSTANT_ALPHA, Src);
			ImguiRadioBlendFunc(BlendFunc::ONE_MINUS_CONSTANT_ALPHA, Src);
			ImGui::Text("Dest");
			ImguiRadioBlendFunc(BlendFunc::ZERO, Dest);
			ImguiRadioBlendFunc(BlendFunc::ONE, Dest);
			ImguiRadioBlendFunc(BlendFunc::ONE_MINUS_SRC_COLOR, Dest);
			ImguiRadioBlendFunc(BlendFunc::DST_COLOR, Dest);
			ImguiRadioBlendFunc(BlendFunc::ONE_MINUS_DST_COLOR, Dest);
			ImguiRadioBlendFunc(BlendFunc::CONSTANT_COLOR, Dest);
			ImguiRadioBlendFunc(BlendFunc::ONE_MINUS_CONSTANT_COLOR, Dest);
			ImguiRadioBlendFunc(BlendFunc::CONSTANT_ALPHA, Dest);
			ImguiRadioBlendFunc(BlendFunc::ONE_MINUS_CONSTANT_ALPHA, Dest);
			app.GetRenderer().SetBlendFunc(Src, Dest);

		}

		ImGui::Text("--Camera Properties--");
		ImGui::DragFloat3("Camera Pos", &app.GetCamera().GetPos().x, .01f);
		ImGui::DragFloat3("Camera Rot", &app.GetCamera().GetRot().x, .01f);
		ImGui::DragFloat("Camera angle", &app.GetCamera().GetAngle(), .01f);
		ImGui::DragFloat3("Camera Scale", &app.GetCamera().GetScale().x, .01f);

		ImGui::ShowDemoWindow((bool*)1);
		ImGui::End();

	}



}

#endif