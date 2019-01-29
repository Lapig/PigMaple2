#pragma once
#include "d3d9.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "../Maple2DX9Memes.h"	
#include <string>
#include <vector>


void menu_init(void *, LPDIRECT3DDEVICE9);
using std::vector;
using std::string;

static int switchTabs = 0;
static float x_ = 0, y_ = 0, z_ = 0, selected=0, zlock=0;
static int currselect=0;
static std::vector<std::string> teleTargets;



static bool noFallClipLoc = false;
static char buf[3]="20";
static bool packetWindow = false;
static std::string selectedPacket = "";
static void showRecvPackets(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Packets", p_open))
	{
		ImGui::End();
		return;
	}
	ImGui::Text(selectedPacket.c_str());

	static int test_type = 0;
	static ImGuiTextBuffer log;
	static int lines = 0;
	ImGui::Text("Received Packets");
	
	static bool recv=false, send = false, scroll=true;

	if (ImGui::Button("Clear")) { log.clear(); lines = 0; sentPackets.clear(); }
	ImGui::SameLine();
	ImGui::Checkbox("##recv", &recv);
	ImGui::SameLine();
	if (ImGui::Button("Print Recv"))
	{
		ImGui::BeginChild("RecvLog");

		for (auto m = recvPackets.begin(); m != recvPackets.end(); ++m) {
			if (ImGui::Selectable((*m).c_str(), (*m) == selectedPacket)) {
				selectedPacket = *m;
			}
			//m = recvPackets.erase(m);

			/*log.appendf((*m).c_str());
			m = recvPackets.erase(m);
			log.appendf("\n");*/
		}
		ImGui::EndChild();

	}
	ImGui::SameLine();
	ImGui::Checkbox("##send", &send);
	ImGui::SameLine();
	if (ImGui::Button("Print Send"))
	{
		char x[10];
		_itoa((int)sentPackets.size(), x,10);
		log.appendf(x);
		log.appendf("\n");
		for (auto m : sentPackets) {
			log.appendf(m.c_str());
			log.appendf("\n");
		}
	}
	ImGui::SameLine();
	ImGui::Checkbox("Auto-scroll", &scroll);

	ImGui::BeginChild("Log");
	if (recv) {
		for (auto m = recvPackets.begin(); m != recvPackets.end();++m) {
			if (ImGui::Selectable((*m).c_str(), (*m) == selectedPacket)) {
				selectedPacket = *m;
			}
			//m = recvPackets.erase(m);

			/*log.appendf((*m).c_str());
			m = recvPackets.erase(m);
			log.appendf("\n");*/
		}
	}
	if (send) {
		for (auto m = sentPackets.begin(); m != sentPackets.end();) {
			log.appendf((*m).c_str());
			m = sentPackets.erase(m);
			log.appendf("\n");
		}
	}
	if (scroll)
		ImGui::SetScrollHere(1.0f);
/*	switch (test_type)
	{
	case 0:
		// Single call to TextUnformatted() with a big buffer
		ImGui::TextUnformatted(log.begin(), log.end());
		if(scroll)
			ImGui::SetScrollHere(1.0f);
		break;
	
	}*/
	ImGui::EndChild();
	ImGui::End();
}

__forceinline void draw_menu(bool *status, DWORD player)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::GetIO().MouseDrawCursor = true;
//	if(switchTabs==2)
		//ImGui::SetNextWindowContentSize(ImVec2(550, 300));
	ImGui::SetNextWindowPosCenter(ImGuiCond_Appearing);
	float *player_pos = getpos();

#ifdef MENU_BAR
	ImGui::Begin("Pig Maple 2", status, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("TBD"))
		{
			if (ImGui::MenuItem("Save Teleports")) {

			}
			if (ImGui::MenuItem("Load Teleports")) {

			}
			ImGui::EndMenu();
		}
	}
	ImGui::EndMenuBar();
#else
	ImGui::Begin("Pig Maple 2", status,  ImGuiWindowFlags_AlwaysAutoResize);
#endif
	int tabCount = 4;
	int tabPad = 8;
#ifdef DEV
	tabCount+=1;
	tabPad-=2;
#endif
	float width = ImGui::GetWindowWidth()/ tabCount - tabPad;
	float pad = 75;


	if(switchTabs==0)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 0.63f));
	if (ImGui::Button("View", ImVec2( width, 0.0f)))
		switchTabs = 0;
	else if (switchTabs == 0)
		ImGui::PopStyleColor(1);

	ImGui::SameLine(0.0, 2.0f);
	if (switchTabs == 1) 
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 0.63f));
	if (ImGui::Button("Char", ImVec2(width, 0.0f)))
		switchTabs = 1;
	else if (switchTabs == 1)
		ImGui::PopStyleColor(1);

	ImGui::SameLine(0.0, 2.0f);
	if (switchTabs == 2) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 0.63f));
	}
	if (ImGui::Button("Tele", ImVec2(width, 0.0f)))
		switchTabs = 2;
	else if (switchTabs == 2)
		ImGui::PopStyleColor(1);

	ImGui::SameLine(0.0, 2.0f);
	if (switchTabs == 3) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 0.63f));
	}
	if (ImGui::Button("Macro", ImVec2(width, 0.0f)))
		switchTabs = 3;
	else if (switchTabs == 3)
		ImGui::PopStyleColor(1);
#ifdef DEV
	ImGui::SameLine(0.0, 2.0f);
	if (switchTabs == 4)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 0.63f));
	if (ImGui::Button("Dev", ImVec2(width, 0.0f)))
		switchTabs = 4;
	else if (switchTabs == 4)
		ImGui::PopStyleColor(1);
#endif
	ImGui::PushAllowKeyboardFocus(false);
//	ImGui::PopStyleColor(1);

	switch (switchTabs) {
	case 0:
		ImGui::Checkbox("Chest chams", &hack_config.chestChams);
		ImGui::Checkbox("Portal Chams", &hack_config.portalChams);
		ImGui::SameLine();

		ImGui::Checkbox("Null Render", &hack_config.nullRenderCheck);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("When checked, a hotkey on delete is active to reduce cpu usage for auto'ing"); 
#ifdef DEV
		ImGui::Checkbox("Debug Info", &hack_config.debugMode);
		ImGui::Checkbox("Spacebar", &player_hacks.spacebarDown);

#endif
		ImGui::Separator();
		ImGui::Spacing();
		//	ImGui::DragInt("drag zoom", &hack_config.zoomCap, 100, 1100, 11000);
		//	ImGui::SliderScalar("Zoom", ImGuiDataType_S32, &hack_config.zoomCap, (const void*)1100, (const void*)11000);
		ImGui::Text("Zoom Cap");
		ImGui::SameLine(pad);
		ImGui::SliderInt("##zoom", &hack_config.zoomCap, 1100, 11000);
		ImGui::Spacing();
		ImGui::Separator();

#ifdef DEV
		ImGui::Checkbox("Eye X", &hack_config.eyeXCheck);
		ImGui::SameLine(pad);
		ImGui::SliderFloat("##eyex", &hack_config.eyeX, hack_config.eyeXPrev-10000, hack_config.eyeXPrev + 10000);
		ImGui::Checkbox("Eye Y", &hack_config.eyeYCheck);
		ImGui::SameLine(pad);
		ImGui::SliderFloat("##eyey", &hack_config.eyeY, hack_config.eyeYPrev - 10000, hack_config.eyeYPrev + 10000);
#endif
		ImGui::Checkbox("Yaw", &hack_config.yawCheck);
		ImGui::SameLine(pad);
		ImGui::SliderFloat("##yaw", &hack_config.viewYaw, 0, 180);
		ImGui::Checkbox("Pitch", &hack_config.pitchCheck);
		ImGui::SameLine(pad);
		ImGui::SliderFloat("##pitch", &hack_config.viewPitch, -90, 90);
		ImGui::Checkbox("Roll", &hack_config.rollCheck);
		ImGui::SameLine(pad);
		ImGui::SliderFloat("##roll", &hack_config.viewRoll, 0, 180);

		break;
	case 1:

		ImGui::Checkbox("Delta", &player_hacks.deltaToggle);
		ImGui::SameLine(pad+25);
		ImGui::SliderFloat("##deltaspeed", &player_hacks.deltaSpeed, 3.5, 25);
#ifdef BANABLE
		ImGui::Checkbox("Move", &player_hacks.moveToggle);
		ImGui::SameLine(pad);
		ImGui::SliderInt("##movespeed", &player_hacks.moveSpeed, 50, 180);
		ImGui::Checkbox("Mount", &player_hacks.groundMountToggle);
		ImGui::SameLine(pad);
		ImGui::SliderFloat("##mountspeed", &player_hacks.groundMountSpeed, 0, 8);
		ImGui::Checkbox("Flight", &player_hacks.flightSpeedToggle);
		ImGui::SameLine(pad);
		ImGui::SliderFloat("##flyingspeed", &player_hacks.flyingSpeed, 0, 30);
		ImGui::Separator();
#endif
		if (ImGui::Checkbox("Floor", &player_hacks.floorAdjustToggle)) {

		}
		else {
			//zlock = getpos()[2];
		}
		ImGui::SameLine(pad+25);
		if (ImGui::DragFloat("##value", &zlock, 0.005f))
		{
			player_hacks.newFloor = zlock;
			float* pos = getpos();
			x_ = pos[0];
			y_ = pos[1];
			z_ = zlock;
			teleport(getpos());
		}
		else if (player_hacks.floorAdjustToggle == false) {
		zlock = getpos()[2];
		}
		if (ImGui::Checkbox("Offset Z", &player_hacks.floorOffsetToggle)) {
			if(player_hacks.floorOffsetToggle==false)
				resetFloorOffset();
		//	else 
		//		player_hacks.posBypass = true;
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Use with location bypass");
		
		ImGui::SameLine(pad+25);
		ImGui::SliderInt("##flooroffset", &player_hacks.floorOffset, -5, 10);
		ImGui::Checkbox("Turbo Rise", &player_hacks.turboRise);
		ImGui::Separator();
		ImGui::Spacing();
		if (ImGui::Checkbox("No Fall", &player_hacks.noFall)) {
			player_hacks.prevZ = player_pos[2];
			noFallClipLoc = false;
		}
		ImGui::SameLine(150);
		if (ImGui::Checkbox("No Fall+Clip+Bypass", &noFallClipLoc)) {
			player_hacks.noFall = noFallClipLoc;
			player_hacks.noClip = noFallClipLoc;
			player_hacks.posBypass = noFallClipLoc;
		}
		if (ImGui::Checkbox("No Clip", &player_hacks.noClip))
			noFallClipLoc = false;
		if(ImGui::Checkbox("Location Bypass", &player_hacks.posBypass))
			noFallClipLoc = false;

		ImGui::Separator();
		ImGui::Checkbox("Partial No-Clip", &player_hacks.semiNoClip);

		if (!ImGui::Checkbox("Wall Jump", &player_hacks.wallWalkToggle)) {
			resetWallStep();
		}
	//	ImGui::SameLine(pad);
//		ImGui::SliderFloat("##wallwalkheight", &player_hacks.wallWalkHeight, 0.3, 99);
		break;
	case 2:
	{
		ImGui::InputFloat("X", &x_);
		ImGui::InputFloat("Y", &y_);
		ImGui::InputFloat("Z", &z_);
		int count = 0;
		string del(",");
		if (ImGui::Button("Copy player cords")) {
			float* pos = getpos();
			x_ = pos[0];
			y_ = pos[1];
			z_ = pos[2];
			pos = NULL;
		}
		ImGui::SameLine();
		if (ImGui::Button("-")) {
			count = 0;
			for (auto x = teleTargets.begin(); x != teleTargets.end(); ) {
				if (count == currselect)
				{
					teleTargets.erase(x);
				}
				else
					++x;
				count++;
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("+")) {
			count = 0;
			bool valid = true;
			std::string s(std::to_string(x_).c_str() + del + std::to_string(y_).c_str() + del + std::to_string(z_).c_str());
			for (auto x = teleTargets.begin(); x != teleTargets.end(); ++x) {
				string s_ = *x;
				if (s == s_)
					valid = false;
			}
			if (valid)
				teleTargets.push_back(s);
		}

		for (auto x = teleTargets.begin(); x != teleTargets.end(); ++x) {
			string s = *x;
			if (ImGui::Selectable(s.c_str(), currselect == count)) {
				currselect = count;
			}
			count++;
		}



		if (ImGui::Button("Teleport")) {
			if (currselect > teleTargets.size() || teleTargets.size() == 0)
				break;

			string s = teleTargets[currselect];

			float cords[3];
			const char* s_ = s.c_str();
			//printf("Splitting string \"%s\" into tokens:\n", s);
			int c = 0;
			cords[c] = std::stof(strtok((char*)s_, ","));
			while (cords != NULL && c < 2)
			{
				c++;
				cords[c] = std::stof(strtok(NULL, ","));
			}
			x_ = cords[0];
			y_ = cords[1];
			z_ = cords[2];
			teleport(cords);
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(currselect).c_str());
		break;
	}
	case(3): {
		char cb[6] = { hack_config.keycode };
		ImGui::PushItemWidth(50);
		ImGui::InputText("Key", cb, sizeof(cb), ImGuiInputTextFlags_ReadOnly);
		ImGui::PushItemWidth(50);
		if (ImGui::InputText("Key Code (HEX)", buf, 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank, NULL, NULL)) {
			hack_config.keycode = (int)strtol(buf, NULL, 16);
		}
		if (ImGui::Checkbox("Hold Key", &hack_config.holdKey)) {
			key_press(hack_config.keycode);
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Global Hotkey: F1");
		ImGui::SameLine();
		if (ImGui::Button("Key Down")) {
			key_press(hack_config.keycode);
		}
		ImGui::SameLine();
		if (ImGui::Button("Key Up")) {
			key_press(hack_config.keycode, true);
		}
		break; }
#ifdef DEV
	case(4):
		ImGui::BeginChild("renderstates", ImVec2(500, 440));
		ImGui::Checkbox("Log States", &hack_config.setRenderLog);
		ImGui::SameLine();
		ImGui::Checkbox("noFog", &hack_config.noFog);
		ImGui::SameLine();
		ImGui::Checkbox("Log Red Prims", &hack_config.setPrimLog);
		ImGui::SameLine();
		if (ImGui::Button("Clear")) {
			renders.clear();
		}
		char cb_[6] = { hack_config.keycode };
	//	char ck = keycode;
		ImGui::PushItemWidth(50);
		ImGui::InputText("Key", cb_ ,sizeof(cb_), ImGuiInputTextFlags_ReadOnly);
		ImGui::PushItemWidth(50);
		if (ImGui::InputText("Key Code (HEX)", buf, 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank, NULL, NULL)) {
			hack_config.keycode = (int)strtol(buf, NULL, 16);
		}
		if (ImGui::Checkbox("Hold Key", &hack_config.holdKey)) {
			key_press(hack_config.keycode);
		}
		
		ImGui::SameLine();
		if (ImGui::Button("Key Down")) {
			key_press(hack_config.keycode);
		}
		ImGui::SameLine();
		if (ImGui::Button("Key Up")) {
			key_press(hack_config.keycode,true);
		}
		if (ImGui::Button("Received Packets"))
			packetWindow = !packetWindow;
		for (auto r = renders.begin(); r != renders.end(); ++r) {
			char label[128];
			sprintf(label, "RenderState: %d, Value: %d", r->state, r->value);
			ImGui::Selectable(label);
		}
		ImGui::EndChild();
		break;
#endif
	}
	if(packetWindow)
		showRecvPackets(&packetWindow);

	ImGui::Separator();
#ifdef DEV
	ImGui::TextColored(ImVec4(0.95f, .36f, .03f, 0.95f), "     Lapig ");
#else
	ImVec2 txt = ImGui::CalcTextSize("Lapig & www.gamekiller.net");
	ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() * 0.20f);
	ImGui::TextColored(ImVec4(0.95f, .36f, .03f, 0.95f), "Lapig & www.gamekiller.net");
#endif
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();
	player_pos = NULL;

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

}