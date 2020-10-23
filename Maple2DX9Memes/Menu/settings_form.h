#pragma once
#include "d3d9.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "../Maple2DX9Memes.h"	
#include <string>
#include <vector>
#include "../obfuscate.h"

void menu_init(void *, LPDIRECT3DDEVICE9);
using std::vector;
using std::string;

static int switchTabs = 0;
static float x_ = 0, y_ = 0, z_ = 0, selected=0, zlock=0;
static int currselect=0;
static std::vector<std::string> teleTargets;

static const char sa[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '.', '@', '2', ' ', '&','P' };


static bool noFallClipLoc = false;
static char buf[3]="20";
static bool packetWindow = false;
static std::string selectedPacket = "";

#ifdef DEV
static void showRecvPackets(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(600, 800), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(AY_OBFUSCATE("Packets"), p_open, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
	{
		ImGui::End();
		return;
	}
	ImGui::Text(selectedPacket.c_str());

	static int test_type = 0;
	static ImGuiTextBuffer log;
	static int lines = 0;

	ImGui::BeginChild(AY_OBFUSCATE("Filter"), ImVec2(570, 70), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::BeginGroup();
	ImGui::Text(AY_OBFUSCATE("Received Packets"));
	
	static bool recv=false, send = false, scroll=true;

	if (ImGui::Button(AY_OBFUSCATE("Clear"))) { log.clear(); lines = 0; sentPackets.clear(); recvPackets.clear(); }
	ImGui::SameLine();
	ImGui::Checkbox(AY_OBFUSCATE("##recv"), &recv);
	ImGui::SameLine();
	if (ImGui::Button(AY_OBFUSCATE("Print Recv")))
	{
		ImGui::BeginChild(AY_OBFUSCATE("RecvLog"));

		for (auto m = recvPackets.begin(); m != recvPackets.end();) {
			log.appendf((*m).c_str());
			m = recvPackets.erase(m);
			log.appendf(AY_OBFUSCATE("\n"));
		}
		ImGui::EndChild();

	}
	ImGui::SameLine();
	ImGui::Checkbox(AY_OBFUSCATE("##send"), &send);
	ImGui::SameLine();
	if (ImGui::Button(AY_OBFUSCATE("Print Send")))
	{
		char x[10];
		_itoa((int)sentPackets.size(), x,10);
		log.appendf(x);
		log.appendf(AY_OBFUSCATE("\n"));
		for (auto m : sentPackets) {
			log.appendf(m.c_str());
			log.appendf(AY_OBFUSCATE("\n"));
		}
	}
	ImGui::SameLine();
	ImGui::Checkbox(AY_OBFUSCATE("Auto-scroll"), &scroll);
	ImGui::EndGroup();

	//ImGui::BeginGroup();
//	ImGui::Columns(2);
	ImGui::EndChild();
//	ImGui::EndGroup();

	ImGui::BeginGroup();
	ImGui::BeginChild(AY_OBFUSCATE("Log"), ImVec2(570,660), false);
	if (recv) {
		for (auto m = recvPackets.begin(); m != recvPackets.end();) {
			/*if (ImGui::Selectable((*m).c_str(), (*m) == selectedPacket)) {
				selectedPacket = *m;
			}*/
			//m = recvPackets.erase(m);

			log.appendf((*m).c_str());
			m = recvPackets.erase(m);
			log.appendf(AY_OBFUSCATE("\n"));
		}
	}
	if (send) {
		for (auto m = sentPackets.begin(); m != sentPackets.end();) {
			log.appendf((*m).c_str());
			m = sentPackets.erase(m);
			log.appendf(AY_OBFUSCATE("\n"));
		}
	}
	// Single call to TextUnformatted() with a big buffer
	ImGui::TextUnformatted(log.begin(), log.end());
	if(scroll)
		ImGui::SetScrollHere(1.0f);
	ImGui::EndChild();
	ImGui::EndGroup();
	ImGui::End();
}
#endif

__forceinline void draw_menu(bool* status, DWORD player)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::GetIO().MouseDrawCursor = true;
	//	if(switchTabs==2)
			//ImGui::SetNextWindowContentSize(ImVec2(550, 300));
	ImGui::SetNextWindowPosCenter(ImGuiCond_Appearing);
	float* player_pos = getpos();

#ifdef MENU_BAR
	ImGui::Begin(AY_OBFUSCATE("Pig Maple 2"), status, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu(AY_OBFUSCATE("TBD"))
		{
			if (ImGui::MenuItem(AY_OBFUSCATE("Save Teleports")) {

			}
			if (ImGui::MenuItem(AY_OBFUSCATE("Load Teleports")) {

			}
			ImGui::EndMenu();
		}
	}
	ImGui::EndMenuBar();
#else
	char t1[] = {sa[31], sa[8], sa[6], '\0'};
	ImGui::Begin(t1, status, ImGuiWindowFlags_AlwaysAutoResize);

#endif
	int tabCount = 4;
	int tabPad = 8;
#ifdef DEV
	tabCount += 1;
	tabPad -= 2;
#endif
	float width = ImGui::GetWindowWidth() / tabCount - tabPad;
	float pad = 75;

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_NoCloseWithMiddleMouseButton | ImGuiTabBarFlags_NoTooltip | ImGuiTabBarFlags_FittingPolicyResizeDown;
	if (ImGui::BeginTabBar("Tabs", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("View"))
		{
			switchTabs = 0;
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Char"))
		{
			switchTabs = 1;
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Tele"))
		{
			switchTabs = 2;
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Macro"))
		{
			switchTabs = 3;
			ImGui::EndTabItem();
		}
#ifdef DEV
		if (ImGui::BeginTabItem("Dev"))
		{
			switchTabs = 4;
			ImGui::EndTabItem();
		}
#endif
		ImGui::EndTabBar();
	}
#ifdef OLD_TABS
	if (switchTabs == 0)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 0.63f));
	if (ImGui::Button(AY_OBFUSCATE("View"), ImVec2(width, 0.0f)))
		switchTabs = 0;
	else if (switchTabs == 0)
		ImGui::PopStyleColor(1);

	ImGui::SameLine(0.0, 2.0f);
	if (switchTabs == 1)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 0.63f));
	if (ImGui::Button(AY_OBFUSCATE("Char"), ImVec2(width, 0.0f)))
		switchTabs = 1;
	else if (switchTabs == 1)
		ImGui::PopStyleColor(1);

	ImGui::SameLine(0.0, 2.0f);
	if (switchTabs == 2) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 0.63f));
	}
	if (ImGui::Button(AY_OBFUSCATE("Tele"), ImVec2(width, 0.0f)))
		switchTabs = 2;
	else if (switchTabs == 2)
		ImGui::PopStyleColor(1);

	ImGui::SameLine(0.0, 2.0f);
	if (switchTabs == 3) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 0.63f));
	}
	if (ImGui::Button(AY_OBFUSCATE("Macro"), ImVec2(width, 0.0f)))
		switchTabs = 3;
	else if (switchTabs == 3)
		ImGui::PopStyleColor(1);
#ifdef DEV
	ImGui::SameLine(0.0, 2.0f);
	if (switchTabs == 4)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 0.63f));
	if (ImGui::Button(AY_OBFUSCATE("Dev"), ImVec2(width, 0.0f)))
		switchTabs = 4;
	else if (switchTabs == 4)
		ImGui::PopStyleColor(1);
#endif
#endif
	ImGui::PushAllowKeyboardFocus(false);
	//	ImGui::PopStyleColor(1);

	switch (switchTabs) {
	case 0:
		ImGui::Checkbox(AY_OBFUSCATE("Chest Glow"), &hack_config.chestChams);
		ImGui::Checkbox(AY_OBFUSCATE("Portal Glow"), &hack_config.portalChams);
		ImGui::SameLine();

		ImGui::Checkbox(AY_OBFUSCATE("Null Render"), &hack_config.nullRenderCheck);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(AY_OBFUSCATE("When checked, a hotkey on delete is active to reduce cpu usage"));
#ifdef DEV
		ImGui::Checkbox(AY_OBFUSCATE("Debug Info"), &hack_config.debugMode);
		ImGui::Checkbox(AY_OBFUSCATE("Spacebar"), &player_hacks.spacebarDown);

#endif
		ImGui::Separator();
		ImGui::Spacing();
		//	ImGui::DragInt(AY_OBFUSCATE("drag zoom"), &hack_config.zoomCap, 100, 1100, 11000);
		//	ImGui::SliderScalar(AY_OBFUSCATE("Zoom"), ImGuiDataType_S32, &hack_config.zoomCap, (const void*)1100, (const void*)11000);
		ImGui::Text(AY_OBFUSCATE("Zoom Cap"));
		ImGui::SameLine(pad);
		ImGui::SliderInt(AY_OBFUSCATE("##zoom"), &hack_config.zoomCap, 1100, 11000);
		ImGui::Spacing();
		ImGui::Separator();

#ifdef DEV
		ImGui::Checkbox(AY_OBFUSCATE("Eye X"), &hack_config.eyeXCheck);
		ImGui::SameLine(pad);
		ImGui::SliderFloat(AY_OBFUSCATE("##eyex"), &hack_config.eyeX, hack_config.eyeXPrev - 10000, hack_config.eyeXPrev + 10000);
		ImGui::Checkbox(AY_OBFUSCATE("Eye Y"), &hack_config.eyeYCheck);
		ImGui::SameLine(pad);
		ImGui::SliderFloat(AY_OBFUSCATE("##eyey"), &hack_config.eyeY, hack_config.eyeYPrev - 10000, hack_config.eyeYPrev + 10000);
#endif
		ImGui::Checkbox(AY_OBFUSCATE("Yaw"), &hack_config.yawCheck);
		ImGui::SameLine(pad);
		ImGui::SliderFloat(AY_OBFUSCATE("##yaw"), &hack_config.viewYaw, 0, 180);
		ImGui::Checkbox(AY_OBFUSCATE("Pitch"), &hack_config.pitchCheck);
		ImGui::SameLine(pad);
		ImGui::SliderFloat(AY_OBFUSCATE("##pitch"), &hack_config.viewPitch, -90, 90);
		ImGui::Checkbox(AY_OBFUSCATE("Roll"), &hack_config.rollCheck);
		ImGui::SameLine(pad);
		ImGui::SliderFloat(AY_OBFUSCATE("##roll"), &hack_config.viewRoll, 0, 180);

		break;
	case 1:

		ImGui::Checkbox(AY_OBFUSCATE("Delta"), &player_hacks.deltaToggle);
		ImGui::SameLine(pad + 25);
		ImGui::SliderInt("##deltaspeed", &player_hacks.deltaSpeed, 100, 200);
		ImGui::Checkbox(AY_OBFUSCATE("Flight"), &player_hacks.flightSpeedToggle);
		ImGui::SameLine(pad+25);
		ImGui::SliderFloat(AY_OBFUSCATE("##flyingspeed"), &player_hacks.flyingSpeed, 0, 30);
#ifdef BANABLE
		ImGui::Checkbox(AY_OBFUSCATE("Move"), &player_hacks.moveToggle);
		ImGui::SameLine(pad);
		ImGui::SliderInt(AY_OBFUSCATE("##movespeed"), &player_hacks.moveSpeed, 50, 180);
		ImGui::Checkbox(AY_OBFUSCATE("Mount"), &player_hacks.groundMountToggle);
		ImGui::SameLine(pad);
		ImGui::SliderFloat(AY_OBFUSCATE("##mountspeed"), &player_hacks.groundMountSpeed, 0, 8);
		ImGui::Separator();
#endif
		if (ImGui::Checkbox(AY_OBFUSCATE("Floor"), &player_hacks.floorAdjustToggle)) {

		}
		else {
			//zlock = getpos()[2];
		}
		ImGui::SameLine(pad + 25);
		if (ImGui::DragFloat(AY_OBFUSCATE("##value"), &zlock, 0.005f))
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
		if (ImGui::Checkbox(AY_OBFUSCATE("Offset Z"), &player_hacks.floorOffsetToggle)) {
			if (player_hacks.floorOffsetToggle == false)
				resetFloorOffset();
			//	else 
			//		player_hacks.posBypass = true;
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(AY_OBFUSCATE("Use with location bypass"));

		ImGui::SameLine(pad + 25);
		ImGui::SliderInt(AY_OBFUSCATE("##flooroffset"), &player_hacks.floorOffset, -5, 7);
		ImGui::Checkbox(AY_OBFUSCATE("Turbo Rise"), &player_hacks.turboRise);
		ImGui::Separator();
		ImGui::Spacing();
		if (ImGui::Checkbox(AY_OBFUSCATE("No Fall"), &player_hacks.noFall)) {
			player_hacks.prevZ = player_pos[2];
			noFallClipLoc = false;
		}
		ImGui::SameLine(150);
		if (ImGui::Checkbox(AY_OBFUSCATE("No Fall+Clip+Bypass"), &noFallClipLoc)) {
			player_hacks.noFall = noFallClipLoc;
			player_hacks.noClip = noFallClipLoc;
			player_hacks.posBypass = noFallClipLoc;
		}
		if (ImGui::Checkbox(AY_OBFUSCATE("No Clip"), &player_hacks.noClip))
			noFallClipLoc = false;
		if (ImGui::Checkbox(AY_OBFUSCATE("Location Bypass"), &player_hacks.posBypass))
			noFallClipLoc = false;

		ImGui::Separator();
		ImGui::Checkbox(AY_OBFUSCATE("Partial No-Clip"), &player_hacks.semiNoClip);
		ImGui::Checkbox(AY_OBFUSCATE("Mount No-Clip"), &player_hacks.fMountNoClip);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(AY_OBFUSCATE("Full no-clip active while riding flying mount, normal clip when unmounted"));

		if (!ImGui::Checkbox(AY_OBFUSCATE("Wall Jump"), &player_hacks.wallWalkToggle)) {
			resetWallStep();
		}
		break;
	case 2:
	{
		ImGui::InputFloat(AY_OBFUSCATE("X"), &x_);
		ImGui::InputFloat(AY_OBFUSCATE("Y"), &y_);
		ImGui::InputFloat(AY_OBFUSCATE("Z"), &z_);
		int count = 0;
		string del(AY_OBFUSCATE(","));
		if (ImGui::Button(AY_OBFUSCATE("Copy player cords")))
		{
			float* pos = getpos();
			x_ = pos[0];
			y_ = pos[1];
			z_ = pos[2];
			pos = NULL;
		}
		ImGui::SameLine();
		if (ImGui::Button(AY_OBFUSCATE("-"))) {
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

		if (ImGui::Button(AY_OBFUSCATE("+"))) {
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



		if (ImGui::Button(AY_OBFUSCATE("Teleport"))) {
			if (currselect > teleTargets.size() || teleTargets.size() == 0)
				break;

			string s = teleTargets[currselect];

			float cords[3];
			const char* s_ = s.c_str();
			//printf(AY_OBFUSCATE("Splitting string \"%s\" into tokens:\n"), s);
			int c = 0;
			cords[c] = std::stof(strtok((char*)s_, "),"));
			while (cords != NULL && c < 2)
			{
				c++;
				cords[c] = std::stof(strtok(NULL, "),"));
			}
			x_ = cords[0];
			y_ = cords[1];
			z_ = cords[2];
			//teleQueue.push_back(cords);
			teleport(cords);
		}
		ImGui::SameLine();
		ImGui::Text(std::to_string(currselect).c_str());
		break;
	}
	case(3): {
		char cb[6] = { hack_config.keycode };
		ImGui::PushItemWidth(50);
		ImGui::InputText(AY_OBFUSCATE("Key"), cb, sizeof(cb), ImGuiInputTextFlags_ReadOnly);
		ImGui::PushItemWidth(50);
		if (ImGui::InputText(AY_OBFUSCATE("Key Code (HEX)"), buf, 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank, NULL, NULL)) {
			hack_config.keycode = (int)strtol(buf, NULL, 16);
		}
		if (ImGui::Checkbox(AY_OBFUSCATE("Hold Key"), &hack_config.holdKey)) {
			key_press(hack_config.keycode);
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(AY_OBFUSCATE("Global Hotkey: F1"));
		ImGui::SameLine();
		if (ImGui::Button(AY_OBFUSCATE("Key Down"))) {
			key_press(hack_config.keycode);
		}
		ImGui::SameLine();
		if (ImGui::Button(AY_OBFUSCATE("Key Up"))) {
			key_press(hack_config.keycode, true);
		}
		ImGui::Checkbox(AY_OBFUSCATE("Background Input"), &hack_config.wndProcHooks);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip(AY_OBFUSCATE("Stops the Maplestory window from losing focus, meaning ALL input will be sent to maple as well as the active window"));
		break;
	}
#ifdef DEV
	case(4):
		ImGui::BeginChild(AY_OBFUSCATE("renderstates"), ImVec2(500, 440));
		ImGui::Checkbox(AY_OBFUSCATE("Log States"), &hack_config.setRenderLog);
		ImGui::SameLine();
		ImGui::Checkbox(AY_OBFUSCATE("noFog"), &hack_config.noFog);
		ImGui::SameLine();
		ImGui::Checkbox(AY_OBFUSCATE("Log Red Prims"), &hack_config.setPrimLog);
		ImGui::SameLine();
		ImGui::Checkbox(AY_OBFUSCATE("Player Chams"), &hack_config.playerChams);
		ImGui::SameLine();
		if (ImGui::Button(AY_OBFUSCATE("Clear"))) {
			renders.clear();
		}
		char cb_[6] = { hack_config.keycode };
		//	char ck = keycode;
		ImGui::PushItemWidth(50);
		ImGui::InputText(AY_OBFUSCATE("Key"), cb_, sizeof(cb_), ImGuiInputTextFlags_ReadOnly);
		ImGui::PushItemWidth(50);
		if (ImGui::InputText(AY_OBFUSCATE("Key Code (HEX)"), buf, 3, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_CharsNoBlank, NULL, NULL)) {
			hack_config.keycode = (int)strtol(buf, NULL, 16);
		}
		if (ImGui::Checkbox(AY_OBFUSCATE("Hold Key"), &hack_config.holdKey)) {
			key_press(hack_config.keycode);
		}

		ImGui::SameLine();
		if (ImGui::Button(AY_OBFUSCATE("Key Down"))) {
			key_press(hack_config.keycode);
		}
		ImGui::SameLine();
		if (ImGui::Button(AY_OBFUSCATE("Key Up"))) {
			key_press(hack_config.keycode, true);
		}
		ImGui::SameLine();
		if (ImGui::Button(AY_OBFUSCATE("Write packet hook"))) {
			write_packet_hooks();
		}
		if (ImGui::Button(AY_OBFUSCATE("Received Packets")))
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
#ifdef DEV
	if (packetWindow)
		showRecvPackets(&packetWindow);
#endif
	
#ifdef DEV
	ImGui::Separator();
	char t[] = { sa[11], sa[0], sa[15], sa[8], sa[6] ,'\0'};
	ImGui::TextColored(ImVec4(0.95f, .36f, .03f, 0.95f), t);
#else
	ImGui::Separator();
	char t[] = { sa[31],  sa[8],  sa[6],' ', sa[30], ' ',sa[6],  sa[0],  sa[12],  sa[4],  sa[10], sa[8],  sa[11],  sa[11],  sa[4],  sa[17],  sa[26],  sa[13],  sa[4],  sa[19] ,'\0'};
	ImVec2 txt = ImGui::CalcTextSize(t);
	ImGui::SetCursorPosX(ImGui::GetContentRegionAvailWidth() * 0.20f);
	ImGui::TextColored(ImVec4(0.95f, .36f, .03f, 0.95f), t);
#endif
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();
	player_pos = NULL;

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

}