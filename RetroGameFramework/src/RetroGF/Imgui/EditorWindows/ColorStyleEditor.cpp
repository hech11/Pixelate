#include "RGFpch.h"
#ifndef RGF_DISTRIBUTE

#include "ColorStyleEditor.h"

#include <IMGUI/imgui.h>

namespace RGF {


	// Theme generator code based on: https://github.com/ocornut/imgui/issues/2265#issuecomment-465432091
	// Again, this is just used for fun.

	static ImVec4 base = ImVec4(0.502f, 0.075f, 0.256f, 1.0f);
	static ImVec4 bg = ImVec4(0.200f, 0.220f, 0.270f, 1.0f);
	static ImVec4 text = ImVec4(0.860f, 0.930f, 0.890f, 1.0f);
	static float high_val = 0.8f;
	static float mid_val = 0.5f;
	static float low_val = 0.3f;
	static float window_offset = -0.2f;

	inline ImVec4 make_high(float alpha) {
		ImVec4 res(0, 0, 0, alpha);
		ImGui::ColorConvertRGBtoHSV(base.x, base.y, base.z, res.x, res.y, res.z);
		res.z = high_val;
		ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
		return res;
	}

	inline ImVec4 make_mid(float alpha) {
		ImVec4 res(0, 0, 0, alpha);
		ImGui::ColorConvertRGBtoHSV(base.x, base.y, base.z, res.x, res.y, res.z);
		res.z = mid_val;
		ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
		return res;
	}

	inline ImVec4 make_low(float alpha) {
		ImVec4 res(0, 0, 0, alpha);
		ImGui::ColorConvertRGBtoHSV(base.x, base.y, base.z, res.x, res.y, res.z);
		res.z = low_val;
		ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
		return res;
	}

	inline ImVec4 make_bg(float alpha, float offset = 0.f) {
		ImVec4 res(0, 0, 0, alpha);
		ImGui::ColorConvertRGBtoHSV(bg.x, bg.y, bg.z, res.x, res.y, res.z);
		res.z += offset;
		ImGui::ColorConvertHSVtoRGB(res.x, res.y, res.z, res.x, res.y, res.z);
		return res;
	}

	inline ImVec4 make_text(float alpha) {
		return ImVec4(text.x, text.y, text.z, alpha);
	}

	void theme_generator() {
		if (!ImGui::Begin("Theme generator")) {
			ImGui::End();
		}
		else {
			ImGui::ColorEdit3("base", (float*)& base, ImGuiColorEditFlags_PickerHueWheel);
			ImGui::ColorEdit3("bg", (float*)& bg, ImGuiColorEditFlags_PickerHueWheel);
			ImGui::ColorEdit3("text", (float*)& text, ImGuiColorEditFlags_PickerHueWheel);
			ImGui::SliderFloat("high", &high_val, 0, 1);
			ImGui::SliderFloat("mid", &mid_val, 0, 1);
			ImGui::SliderFloat("low", &low_val, 0, 1);
			ImGui::SliderFloat("window", &window_offset, -0.4f, 0.4f);

			ImGuiStyle& style = ImGui::GetStyle();

			style.Colors[ImGuiCol_Text] = make_text(0.78f);
			style.Colors[ImGuiCol_TextDisabled] = make_text(0.28f);
			style.Colors[ImGuiCol_WindowBg] = make_bg(1.00f, window_offset);
			style.Colors[ImGuiCol_ChildBg] = make_bg(0.58f);
			style.Colors[ImGuiCol_PopupBg] = make_bg(0.9f);
			style.Colors[ImGuiCol_Border] = make_bg(0.6f, -0.05f);
			style.Colors[ImGuiCol_BorderShadow] = make_bg(0.0f, 0.0f);
			style.Colors[ImGuiCol_FrameBg] = make_bg(1.00f);
			style.Colors[ImGuiCol_FrameBgHovered] = make_mid(0.78f);
			style.Colors[ImGuiCol_FrameBgActive] = make_mid(1.00f);
			style.Colors[ImGuiCol_TitleBg] = make_low(1.00f);
			style.Colors[ImGuiCol_TitleBgActive] = make_high(1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = make_bg(0.75f);
			style.Colors[ImGuiCol_MenuBarBg] = make_bg(0.47f);
			style.Colors[ImGuiCol_ScrollbarBg] = make_bg(1.00f);
			style.Colors[ImGuiCol_ScrollbarGrab] = make_low(1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = make_mid(0.78f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = make_mid(1.00f);
			style.Colors[ImGuiCol_CheckMark] = make_high(1.00f);
			style.Colors[ImGuiCol_SliderGrab] = make_bg(1.0f, .1f);
			style.Colors[ImGuiCol_SliderGrabActive] = make_high(1.0f);
			style.Colors[ImGuiCol_Button] = make_bg(1.0f, .2f);
			style.Colors[ImGuiCol_ButtonHovered] = make_mid(1.00f);
			style.Colors[ImGuiCol_ButtonActive] = make_high(1.00f);
			style.Colors[ImGuiCol_Header] = make_mid(0.76f);
			style.Colors[ImGuiCol_HeaderHovered] = make_mid(0.86f);
			style.Colors[ImGuiCol_HeaderActive] = make_high(1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
			style.Colors[ImGuiCol_ResizeGripHovered] = make_mid(0.78f);
			style.Colors[ImGuiCol_ResizeGripActive] = make_mid(1.00f);
			style.Colors[ImGuiCol_PlotLines] = make_text(0.63f);
			style.Colors[ImGuiCol_PlotLinesHovered] = make_mid(1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = make_text(0.63f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = make_mid(1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = make_mid(0.43f);
			style.Colors[ImGuiCol_ModalWindowDimBg] = make_bg(0.73f);
			style.Colors[ImGuiCol_Tab] = make_bg(0.40f);
			style.Colors[ImGuiCol_TabHovered] = make_high(1.00f);
			style.Colors[ImGuiCol_TabActive] = make_mid(1.00f);
			style.Colors[ImGuiCol_TabUnfocused] = make_bg(0.40f);
			style.Colors[ImGuiCol_TabUnfocusedActive] = make_bg(0.70f);
			style.Colors[ImGuiCol_DockingPreview] = make_high(0.30f);

			if (ImGui::Button("Export")) {
				ImGui::LogToTTY();
				ImGui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;\n");
				for (int i = 0; i < ImGuiCol_COUNT; i++) {
					const ImVec4& col = style.Colors[i];
					const char* name = ImGui::GetStyleColorName(i);
					ImGui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);\n",
						name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
				}
				ImGui::LogFinish();
			}
			ImGui::End();
		}
		
	}



	void ColorStyleEditor::Init() {
	}
	void ColorStyleEditor::ShutDown() {
	}

	static void ChangeToDarkTheme() {
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 0.78f);
		colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 1.00f, 1.00f, 0.28f);
		colors[ImGuiCol_WindowBg] = ImVec4(-0.25f, -0.25f, -0.25f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.58f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.90f);
		colors[ImGuiCol_Border] = ImVec4(0.10f, 0.10f, 0.10f, 0.60f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.15f, 0.15f, 0.15f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.78f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.15f, 0.75f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.47f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.78f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.43f, 0.43f, 0.43f, 0.76f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.86f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.78f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 0.40f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.15f, 0.40f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 0.70f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.52f, 0.52f, 0.52f, 0.30f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 0.63f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 1.00f, 1.00f, 0.63f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.43f, 0.43f, 0.43f, 0.43f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.73f);
	}

	static void ChangeToLightTheme() {
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 0.78f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.00f, 0.00f, 0.28f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.57f, 0.57f, 0.57f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.77f, 0.77f, 0.77f, 0.58f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.77f, 0.77f, 0.77f, 0.90f);
		colors[ImGuiCol_Border] = ImVec4(0.72f, 0.72f, 0.72f, 0.60f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.77f, 0.77f, 0.77f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.77f, 0.77f, 0.77f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.83f, 0.83f, 0.83f, 0.78f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.83f, 0.83f, 0.83f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.77f, 0.77f, 0.77f, 0.75f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.77f, 0.77f, 0.77f, 0.47f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.77f, 0.77f, 0.77f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.83f, 0.83f, 0.83f, 0.78f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.83f, 0.83f, 0.83f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.97f, 0.97f, 0.97f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.83f, 0.83f, 0.83f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.83f, 0.83f, 0.83f, 0.76f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.83f, 0.83f, 0.83f, 0.86f);
		colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.83f, 0.83f, 0.83f, 0.78f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.83f, 0.83f, 0.83f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.77f, 0.77f, 0.77f, 0.40f);
		colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.83f, 0.83f, 0.83f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.77f, 0.77f, 0.77f, 0.40f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.77f, 0.77f, 0.77f, 0.70f);
		colors[ImGuiCol_DockingPreview] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 0.00f, 0.00f, 0.63f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.83f, 0.83f, 0.83f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.00f, 0.00f, 0.63f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.83f, 0.83f, 0.83f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.83f, 0.83f, 0.83f, 0.43f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.77f, 0.77f, 0.77f, 0.73f);
	}

	static void ChangeToDarkPink() {
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 0.78f);
		colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 1.00f, 1.00f, 0.28f);
		colors[ImGuiCol_WindowBg] = ImVec4(-0.05f, -0.03f, -0.04f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.06f, 0.08f, 0.58f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.09f, 0.06f, 0.08f, 0.90f);
		colors[ImGuiCol_Border] = ImVec4(0.04f, 0.03f, 0.04f, 0.60f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.09f, 0.06f, 0.08f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.09f, 0.06f, 0.08f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.62f, 0.09f, 0.31f, 0.78f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.62f, 0.09f, 0.31f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.45f, 0.07f, 0.23f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.66f, 0.10f, 0.34f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.09f, 0.06f, 0.08f, 0.75f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.09f, 0.06f, 0.08f, 0.47f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.09f, 0.06f, 0.08f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.45f, 0.07f, 0.23f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.62f, 0.09f, 0.31f, 0.78f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.62f, 0.09f, 0.31f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.66f, 0.10f, 0.34f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.19f, 0.13f, 0.16f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.10f, 0.34f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.29f, 0.19f, 0.24f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.62f, 0.09f, 0.31f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.66f, 0.10f, 0.34f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.62f, 0.09f, 0.31f, 0.76f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.62f, 0.09f, 0.31f, 0.86f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.66f, 0.10f, 0.34f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.62f, 0.09f, 0.31f, 0.78f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.62f, 0.09f, 0.31f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.09f, 0.06f, 0.08f, 0.40f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.66f, 0.10f, 0.34f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.62f, 0.09f, 0.31f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.09f, 0.06f, 0.08f, 0.40f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.09f, 0.06f, 0.08f, 0.70f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.66f, 0.10f, 0.34f, 0.30f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 0.63f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.62f, 0.09f, 0.31f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 1.00f, 1.00f, 0.63f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.62f, 0.09f, 0.31f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.62f, 0.09f, 0.31f, 0.43f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.09f, 0.06f, 0.08f, 0.73f);
	}
	void ColorStyleEditor::OnImguiRender() {
		if (IsOpen) {
			static bool ThemeGenOpen = false;
			static bool DarkTheme = false;
			static bool LightTheme = false;
			static bool DarkPinkTheme = false;
			
			if (!ImGui::Begin("Themes", &IsOpen)) {
				ImGui::End();
			}
			else {
				if (ImGui::Button("Dark Theme")) {
					DarkTheme = true;
				}
				if (ImGui::Button("Light Theme")) {
					LightTheme = true;
				}
				if (ImGui::Button("Dark Pink Theme")) {
					DarkPinkTheme = true;
				}
				if (ImGui::Button("Theme Generator")) {
					ThemeGenOpen = !ThemeGenOpen;
				}
				ImGui::End();
				if (ThemeGenOpen) {
					theme_generator();
				}
				if (DarkTheme) {
					ChangeToDarkTheme();
					DarkTheme = false;
				}
				if (LightTheme) {
					ChangeToLightTheme();
					LightTheme = false;
				}
				if (DarkPinkTheme) {
					ChangeToDarkPink();
					DarkPinkTheme = false;
				}

			}
		}


	}
}

#endif