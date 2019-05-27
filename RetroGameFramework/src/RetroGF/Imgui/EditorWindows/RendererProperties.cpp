#include "RGFpch.h"

#ifndef RGF_DISTRIBUTE

#include "RendererProperties.h"

#include <IMGUI/imgui.h>

#include <GLAD/include/glad.h>
#include <GLFW/include/GLFW/glfw3.h>

#include "RetroGF/Application.h"
#include "RetroGF/Rendering/Renderer.h"




namespace RGF {

	void RendererProperties::Init()  {

	}
	void RendererProperties::ShutDown() {
	}

	

	void RendererProperties::OnImguiRender() {

		ImGui::Begin("Renderer Properties");

		Application& app = Application::GetApp();

		static float rgbClear[3] = { 0, 0, 0 };
		ImGui::ColorEdit3("Clear Color", rgbClear);
		app.GetRenderer().ClearColor(rgbClear[0], rgbClear[1], rgbClear[2]);

		static bool depthTest = false;
		static bool stencilTest = false;
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

		ImGui::End();
	}



}

#endif