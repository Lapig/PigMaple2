#pragma once
#include "settings_form.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <string>

int str2intbool(std::string s) {
	return (s == "1") ? 1 : 0;
}
void str2floatarr(std::string s, float out[]) {
	char * pch;
	pch = strtok((char*)s.c_str(), ",");
	//float out[3];
	int tokenc = 0;
	while (pch != NULL)
	{
		if (tokenc >= 3)
			return;
		out[tokenc] = std::strtof(pch,NULL);
		pch = strtok(NULL, ",");
		tokenc++;
	}
}

void *hwnd;
void menu_init(void *game_hwnd, LPDIRECT3DDEVICE9 Device)
{
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(game_hwnd);
	ImGui_ImplDX9_Init(Device);
	ImGuiIO &io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	io.IniFilename = NULL;
	//io.DisplaySize = ImVec2(vp.Height, vp.Width);
	hwnd = game_hwnd;
	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.0;
//	style.ChildWindowRounding = 3;
	style.WindowRounding = 3.0;
	style.GrabRounding = 1;
	style.GrabMinSize = 20;
	style.FrameRounding = 0;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ItemInnerSpacing = ImVec2(6, 6);
	style.ItemSpacing = ImVec2(12, 6);
	style.WindowPadding = ImVec2(8, 6);
	style.TabRounding = 1.f;

	//ImColor mainColor = ImColor(54, 54, 54, 255);
	//ImColor bodyColor = ImColor(24, 24, 24, 240);

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 0.95f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.12f, 0.12f, 1.00f);
	colors[ImGuiCol_ChildWindowBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.53f, 0.53f, 0.53f, 0.46f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.85f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.22f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 0.53f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.2f, 0.65f, 0.17f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.48f, 0.47f, 0.47f, 0.91f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.55f, 0.55f, 0.62f);
	colors[ImGuiCol_Button] = ImVec4(0.50f, 0.50f, 0.50f, 0.63f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.67f, 0.67f, 0.68f, 0.63f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.26f, 0.26f, 0.63f);
	colors[ImGuiCol_Header] = ImVec4(0.54f, 0.54f, 0.54f, 0.58f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.64f, 0.65f, 0.65f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
	colors[ImGuiCol_Separator] = ImVec4(0.58f, 0.58f, 0.58f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.64f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.81f, 0.81f, 0.81f, 0.64f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.87f, 0.87f, 0.87f, 0.53f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.87f, 0.87f, 0.87f, 0.74f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.87f, 0.87f, 0.87f, 0.74f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.68f, 0.68f, 0.68f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.77f, 0.33f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.87f, 0.55f, 0.08f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.47f, 0.60f, 0.76f, 0.47f);
	colors[ImGuiCol_Tab] = ImVec4(0.54f, 0.54f, 0.54f, 0.58f);
	colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.64f, 0.65f, 0.65f, 0.80f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.54f, 0.54f, 0.54f, 0.58f);

	colors[ImGuiCol_COUNT] = ImVec4(0.26f, 0.26f, 0.26f, 0.63f);

#ifdef DEV
//	teleTargets.push_back("2.648437,-2.695625,2.766172");
//	teleTargets.push_back("-2.084667,-3.101826,2.641719");
#endif
}
