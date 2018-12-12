#pragma once
#include "d3d9.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "../Maple2DX9Memes.h"	

void menu_init(void *, LPDIRECT3DDEVICE9);


__forceinline void draw_menu(bool *status)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::GetIO().MouseDrawCursor = true;


	ImGui::Begin("Settings", status, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);


	ImGui::Checkbox("Chest chams", &hack_config.chestChams);
	ImGui::Checkbox("Portal Chams", &hack_config.portalChams);
	ImGui::Checkbox("Wireframe", &hack_config.wireFrame);

	/*if (ImGui::Button("Refresh Camera Hook"))
	{
		hookCamera(true);
	}*/
	ImGui::Separator();
	ImGui::Spacing();

//	ImGui::DragInt("drag zoom", &hack_config.zoomCap, 100, 1100, 11000);
//	ImGui::SliderScalar("Zoom", ImGuiDataType_S32, &hack_config.zoomCap, (const void*)1100, (const void*)11000);
	ImGui::SliderInt("Zoom", &hack_config.zoomCap, 1100, 11000);
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Checkbox("Yaw Ctrl", &hack_config.yawCheck);
	ImGui::SliderFloat("Yaw", &hack_config.viewYaw, 0, 180);
	ImGui::Checkbox("Pitch Ctrl", &hack_config.pitchCheck);
	ImGui::SliderFloat("Pitch", &hack_config.viewPitch, -90, 90);
	ImGui::Checkbox("Roll Ctrl", &hack_config.rollCheck);
	ImGui::SliderFloat("Roll", &hack_config.viewRoll, 0, 180);



	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

}