#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#include <detours.h>
#include <sstream>
#include <string>
#include "Asm.h"
#include "Menu/imgui.h"
#include "Maple2DX9Memes.h"
#include "Menu/settings_form.h"
#include "dxdraw.h"
#include <vector>

using namespace Asm;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT(__stdcall * tDrawIndexedPrimitive)(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
tDrawIndexedPrimitive oDrawIndexedPrimitive;

typedef HRESULT(__stdcall * tEndScene)(LPDIRECT3DDEVICE9 Device);
tEndScene oEndScene;

typedef HRESULT(__stdcall * tReset)(LPDIRECT3DDEVICE9 Device, D3DPRESENT_PARAMETERS* pPresentationParameters);
tReset oReset;

//device
LPDIRECT3DDEVICE9 Device;

//init once
bool FirstInit = false;
bool writeOnce = TRUE;
bool m_bCreated = false;
//vertexshader
IDirect3DVertexShader9* vShader;

UINT vSize;

//viewport
D3DVIEWPORT9 viewport;

//
UINT mStartRegister;
UINT mVector4fCount;

//font
LPD3DXFONT dxFont;

//shader
IDirect3DPixelShader9 *sGreen = NULL;
IDirect3DPixelShader9 *sRed = NULL;

IDirect3DPixelShader9 **shaderPrev;
char ShaderRed[] = "ps_2_0 def c0, 1.0, 0.0, 0.0, 0.8 mov oC0, c0 mov oDepth, c0.b";
char ShaderGreen[] = "ps_2_0 def c0, 0.0, 1.0, 0.0, 1.0 mov oC0, c0 mov oDepth, c0.b";
ID3DXBuffer *ShaderBufferColor = NULL;
ID3DXBuffer *ShaderBufferWeapon = NULL;

WNDPROC game_wndprc;
HWND game_hwnd;

static float zoomCap = 1100;
static float roll = 90;
static float yaw = 45;
static float pitch = -40.89387131f;

static bool MENU_DISPLAYING = false;
uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
//=================

#define jmp(frm, to) (int)(((int)to - (int)frm) - 5);
DWORD camHookEntry = 0x0;	//MapleStory2.exe+2D992C - 8B 90 88000000        - mov edx,[eax+00000088]	8b 90 88 00 00 00 ff ? b0 ?
DWORD camHookExit = 0x0;
DWORD playerBaseEntry = 0x00B07708;	//MapleStory2.exe+707708 - 8B 80 24010000        - mov eax,[eax+00000124]
DWORD playerBaseExit = playerBaseEntry + 6;
static DWORD cameraPtr = 0x0, playerPtr = 0x0;
static DWORD cameraPtrBase = 0x0;
static DWORD cameraPtrPrev = 0x0;
std::vector<unsigned int> offsets = { 0x124,0x88 };

void drawPigHud(LPDIRECT3DDEVICE9 device)
{
	FillRGB(device, 0, 0, 115, 40, D3DCOLOR_ARGB(200, 34, 34, 34));
	dxDrawText(dxFont, 5, 20, 0xFF00FFE8, (CHAR*)"Pigbot", DT_CENTER);

	std::string aimstr("Aim Key: ");
	aimstr.append("Q");
	dxDrawText(dxFont, 1, 5, 0xFFFFFFFF, (CHAR*)aimstr.c_str());
}

void _declspec(naked) _playerBaseHook() {
	_asm {
		mov dword ptr[playerPtr], eax
		mov eax, [eax + 0x124]
		jmp dword ptr[playerBaseExit]
	}
}

static bool hookFlag = 0;
void _declspec(naked) _cameraHook() {
	_asm {
		mov dword ptr[cameraPtr], ecx
		mov edx, [eax + 0x88]
		jmp dword ptr[camHookExit]
	}
}

int hookPlayer(bool state)
{
	if (state) {
		DWORD old;
		VirtualProtect((LPVOID)(playerBaseEntry), sizeof(BYTE) * 6, PAGE_EXECUTE_READWRITE, &old);
		*(BYTE*)playerBaseEntry = 0xE9;
		*(DWORD*)(playerBaseEntry + 1) = jmp(playerBaseEntry, _cameraHook);
		*(BYTE*)(playerBaseEntry + 5) = 0x90;
		VirtualProtect((LPVOID)(playerBaseEntry), sizeof(BYTE) * 6, old, &old);
	}
	else
	{
		DWORD old;
		VirtualProtect((LPVOID)(playerBaseEntry), sizeof(BYTE) * 6, PAGE_EXECUTE_READWRITE, &old);
		BYTE orig[] = { 0x8B, 0x80, 0x24, 0x01, 0x00, 0x00 };
		for (int i = 0; i < sizeof(orig); i++) {
			*(BYTE*)(playerBaseEntry + i) = orig[i];
		}
		VirtualProtect((LPVOID)(playerBaseEntry), sizeof(BYTE) * 6, old, &old);
	}
	return 1;
}
int hookCamera(bool state) {
	return 0;
	cameraPtr = (DWORD)&cameraPtr;

	if (hookFlag == state)
		return 0;
	hookFlag = state;
	return 0;
	if (state) {
		DWORD old;
		VirtualProtect((LPVOID)(camHookEntry), sizeof(BYTE) * 6, PAGE_EXECUTE_READWRITE, &old);
		*(BYTE*)camHookEntry = 0xE9;
		*(DWORD*)(camHookEntry + 1) = jmp(camHookEntry, _cameraHook);
		*(BYTE*)(camHookEntry + 5) = 0x90;
		VirtualProtect((LPVOID)(camHookEntry), sizeof(BYTE) * 6, old, &old);
	}
	else {
		DWORD old;
		VirtualProtect((LPVOID)(camHookEntry), sizeof(BYTE) * 6, PAGE_EXECUTE_READWRITE, &old);
		BYTE orig[] = { 0x8B, 0x90, 0x88, 0x00, 0x00, 0x00 };
		for (int i = 0; i < sizeof(orig); i++) {
			*(BYTE*)(camHookEntry + i) = orig[i];
		}
		zoomCap = 1100;
		roll = *(float*)(cameraPtr + 0xC4);
		yaw = *(float*)(cameraPtr + 0xC8);
		pitch = *(float*)(cameraPtr + 0xCC);
		VirtualProtect((LPVOID)(camHookEntry), sizeof(BYTE) * 6, old, &old);
	}

	return 1;
}
bool UIPrim(UINT NumVertices, UINT primCount) {
	return ((NumVertices == 32 && primCount == 40) || (NumVertices == 24 && primCount == 24) ||
		(NumVertices == 32 && primCount == 40) ||
		(NumVertices == 472 && primCount == 236) ||
		(NumVertices == 8 && primCount == 10) ||
		(NumVertices == 52 && primCount == 26) ||
		(NumVertices == 32 && primCount == 36) ||
		(NumVertices == 96 && primCount == 48) ||
		(NumVertices == 276 && primCount == 138) ||
		(NumVertices == 128 && primCount == 64) ||
		(NumVertices == 500 && primCount == 250) ||
		(NumVertices == 388 && primCount == 194) ||
		(NumVertices == 28 && primCount == 14) ||
		(NumVertices == 16 && primCount == 20) ||
		(NumVertices == 24 && primCount == 30) ||
		(NumVertices == 4 && primCount <= 3) ||
		(NumVertices == 230 && primCount == 306) ||
		(NumVertices == 180 && primCount == 266) ||
		(NumVertices == 16 && primCount == 18) ||
		(NumVertices == 64 && primCount == 32) ||
		(NumVertices == 48 && primCount == 24) ||
		(NumVertices == 12 && primCount == 6) ||
		(NumVertices == 24 && primCount == 12) ||
		(NumVertices == 72 && primCount == 36) ||
		(NumVertices == 20 && primCount == 10) ||
		(NumVertices == 56 && primCount == 28) ||
		(NumVertices == 68 && primCount == 34) ||
		(NumVertices == 112 && primCount == 56) ||
		(NumVertices == 16 && primCount == 12) ||
		(NumVertices == 152 && primCount == 76) ||
		(NumVertices == 16 && primCount == 8) ||
		(NumVertices == 32 && primCount == 16) ||
		(NumVertices == 76 && primCount == 38) ||
		(NumVertices == 36 && primCount == 18) ||
		(NumVertices == 8 && primCount == 6) ||
		(NumVertices == 32 && primCount == 30) ||
		(NumVertices == 40 && primCount == 20) ||
		(NumVertices == 500 && primCount == 250) ||
		(NumVertices == 44 && primCount == 22) ||
		(NumVertices == 352 && primCount == 176) ||
		(NumVertices == 171 && primCount == 306) ||
		(NumVertices == 124 && primCount == 62) ||
		(NumVertices == 80 && primCount == 40) ||
		(NumVertices > 1250 || primCount > 550) ||	//chat text
		(NumVertices == 72 && primCount == 90) ||
		(NumVertices == 168 && primCount == 84) ||
		(NumVertices == 120 && primCount == 60) ||
		(NumVertices == 224 && primCount == 112));
}

bool probablyUIPrim(UINT NumVertices, UINT primCount) {
	return (NumVertices < 10 || primCount < 8) || (NumVertices == 48 && primCount == 60) ||
		(NumVertices == 171 && primCount == 306) ||
		(NumVertices == 602 && primCount == 856) ||
		(NumVertices == 402 && primCount == 600) ||
		(NumVertices == 310 && primCount == 486);
}

DWORD zEnable = 1;
DWORD wireFrame = 0;
DWORD showDebug = 0;
bool cameraHookState = false;
bool shouldRefreshCamera = false;

HRESULT __stdcall hkDrawIndexedPrimitive(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	HRESULT hRet;
	float pSize = 50;
	if (Device == nullptr)
		return oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

	if (FirstInit == FALSE)
	{
		D3DXAssembleShader(ShaderGreen, sizeof(ShaderGreen), NULL, NULL, 0, &ShaderBufferColor, NULL);
		Device->CreatePixelShader((const DWORD*)ShaderBufferColor->GetBufferPointer(), &sGreen);
		ShaderBufferColor->Release();

		D3DXAssembleShader(ShaderRed, sizeof(ShaderRed), NULL, NULL, 0, &ShaderBufferWeapon, NULL);
		Device->CreatePixelShader((const DWORD*)ShaderBufferWeapon->GetBufferPointer(), &sRed);
		ShaderBufferWeapon->Release();

		camHookEntry = AobScan("8B 90 88 00 00 00 FF ?? B0 ??");
		camHookExit = camHookEntry + 0x6;
		Device->GetViewport(&viewport);

		FirstInit = TRUE;
	}

	DWORD alpharef, imguiFlag = 0x0;
	Device->GetRenderState(D3DRS_STENCILFAIL, &imguiFlag);	//6 indicates imgui call
	Device->GetRenderState(D3DRS_ALPHAREF, &alpharef);
	//skip ui components
	if (UIPrim(NumVertices, primCount) || probablyUIPrim(NumVertices, primCount) || alpharef == 0x00000001) {
		return oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}
	if (imguiFlag == 6)
		return oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

	//get stride
	LPDIRECT3DVERTEXBUFFER9 StreamData;
	UINT OffsetInBytes;
	UINT Stride;
	if (Device->GetStreamSource(0, &StreamData, &OffsetInBytes, &Stride) == D3D_OK)
	{
		if (StreamData != NULL)
			StreamData->Release();
	}

	//get vSize
/*	hRet = Device->GetVertexShader(&vShader);
	if (SUCCEEDED(hRet) && vShader != NULL)
	{
		hRet = vShader->GetFunction(NULL, &vSize);
		if (SUCCEEDED(hRet) && vShader != NULL)
		{
			vShader->Release();
			vShader = NULL;
		}
	}
	*/

	if ((Stride == 20 || Stride == 16))//text? chat at least
	{
		return oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}

	Device->GetRenderState(D3DRS_ZENABLE, &zEnable);

	if (hack_config.portalChams && (NumVertices == 144 && primCount == 280)) {	//portals
		Device->SetRenderState(D3DRS_ZENABLE, 0);
		Device->SetPixelShader(sGreen);
	}
	else if ((NumVertices == 291 && primCount == 424)/* || (NumVertices == 128 && primCount == 92) */) //treasure chest bottom 1/2 of model or full model at distance
	{
		Device->SetRenderState(D3DRS_ZENABLE, 0);
		Device->SetPixelShader(sRed);
		//	hRet = oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

		//	chestCount += 1;
		//	return hRet;
	}
	else if (hack_config.wireFrame) {
		Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		hRet = oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		return hRet;
	}
	Device->SetRenderState(D3DRS_ZENABLE, zEnable);
	return oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

//=====================================================================================
float pointx = 0, pointy = 0;
HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 Device)
{
	if (Device == nullptr)
		return oEndScene(Device);
	if (!m_bCreated)
	{
		m_bCreated = true;
		D3DXCreateFontA(Device, 14, 0, FW_BOLD, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas", &dxFont);

		D3DDEVICE_CREATION_PARAMETERS CParams;
		Device->GetCreationParameters(&CParams);
		//auto g_oWndProc = reinterpret_cast<WNDPROC>(GetWindowLongPtr(CParams.hFocusWindow, GWL_WNDPROC, reinterpret_cast<LONG>(WndProc)));
		game_wndprc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(CParams.hFocusWindow, GWLP_WNDPROC, (LONG_PTR)WndProc));

		menu_init(game_hwnd, Device);
	}
	//	FillRGB(Device, 70, 390, 70, 70, D3DXVECTOR4(34, 34, 34, 200));
	//	dxDrawText(dxFont, 97, 400, 0xFFFFFFFF, "PIG");
	//	dxDrawText(dxFont, 100, 425, 0xFFFF0000, std::to_string(prevChestCount).c_str());

	if (GetAsyncKeyState(0x2E) & 1) {//delete
		if (showDebug == 0)
			showDebug = 1;
		else
			showDebug = 0;
	}

	//	if (GetAsyncKeyState(0x21) & 1) { //page up
	////		cameraHookState = !cameraHookState;
	//	}
	if ((GetAsyncKeyState(0x2D) & 1)) {
		MENU_DISPLAYING = !MENU_DISPLAYING;
		if (MENU_DISPLAYING && cameraHookState != hookFlag)
		{
			/*	hookCamera(true);
				Sleep(60);
				hookCamera(false);*/
		}
	}
	/*	if (cameraHookState != hookFlag) {
			hookCamera(cameraHookState);
		}*/
	if (MENU_DISPLAYING)
	{
		//insert
		draw_menu(&MENU_DISPLAYING);
		if (!hookFlag)
			hookCamera(true);
	}
	else if (hookFlag)
	{
		hookCamera(false);
	}
	//	preScene = true;
	//	bool xUpdate = false, yUpdate = false;

		/*if (!hookFlag && !showDebug)
			return oEndScene(Device);*/
			/*	if (cameraPtr != cameraPtrPrev) {
					cameraPtrPrev = cameraPtr;

					zoomCap = 1100;
					eyex = 0;
					eyey = 0;
					roll = 90;
					yaw = 45;
					pitch = -40.89387131f;
				}
				float eyeXOffset = 0;
				float eyeYOffset = 0;*/

	if (GetAsyncKeyState(0x6B) & 0x8000) {//numpad +
		hookCamera(true);
		//zoomCap += 1000;
	}
	if (GetAsyncKeyState(0x6D) & 1) {//numpad -
		zoomCap -= 1000;
	}
#ifdef WORLD2SCREEN
	if (GetAsyncKeyState(0x62) & 0x8800) {//numpad 2
		pointx += .1; pointy += .1;
	}
	if (GetAsyncKeyState(0x63) & 0x8800) {//numpad 3
		pointx -= .1; pointy -= .1;
	}
	//	dxDrawText(Device, 100, 500, 0xFFFFFFFF, std::to_string(pointx).c_str());
	//	dxDrawText(Device, 100, 530, 0xFFFFFFFF, std::to_string(pointy).c_str());
	if (GetAsyncKeyState(0x61) & 0x8000) {
		D3DXVECTOR3 screenpos = world2Screen(Device, pointx, pointy, 2.19187);
		FillRGB(Device, screenpos.x, screenpos.y, 25, 25, D3DXVECTOR4(255, 22, 22, 255));

		/*
				float recp = 1.0f / screenpos.w;

				float ScreenX = 0, ScreenY = 0;
			//	if (screenpos.w >= 0.001f)
				{
					ScreenX = (1 + (screenpos.x*recp))  * (viewport.Width / 2);
					ScreenY = (1 - (screenpos.y*recp))  * (viewport.Height / 2);
				}
				FillRGB(Device, ScreenX, ScreenY, 25, 25, D3DXVECTOR4(255,22,22,255));*/
	}
#endif
#ifdef angleControl
	i
		if (GetAsyncKeyState(0x64) & 1) {//numpad 4
			eyeYOffset += 500;
			yUpdate = true;
		}
	if (GetAsyncKeyState(0x66) & 1) {//numpad 6
		eyeYOffset -= 500;
		yUpdate = true;
	}

	if (GetAsyncKeyState(0x70) & 0x8000) {//f1-6
		roll += 1;
	}
	if (GetAsyncKeyState(0x71) & 0x8000) {//
		roll -= 1;
	}
	if (GetAsyncKeyState(0x72) & 0x8000) {//
		yaw += 1;
	}
	if (GetAsyncKeyState(0x73) & 0x8000) {//
		yaw -= 1;
	}
	if (GetAsyncKeyState(0x74) & 0x8000) {//
		pitch += 1;
	}
	if (GetAsyncKeyState(0x75) & 0x8000) {//
		pitch -= 1;
	}
#endif

	/*		ss << std::hex << cameraPtr;
			line = ss.str(); ss.str(std::string());
			dxDrawText(Device, 150, 420, 0xFFFFFFFF, (char*)std::to_string(registerNum).c_str());
			D3DXMATRIX ViewProjectionMatrix;
			Device->GetVertexShaderConstantF(registerNum, ViewProjectionMatrix, 4);//change this to match your game matrix
			ss <<  ViewProjectionMatrix._11 << ", " << ViewProjectionMatrix._12 << ", " << ViewProjectionMatrix._13 << ", " << ViewProjectionMatrix._14;
			line = ss.str(); ss.str(std::string());
			dxDrawText(Device, 100, 450, 0xFFFFFFFF, (char*)line.c_str());
			ss <<  ViewProjectionMatrix._21 << ", " << ViewProjectionMatrix._22 << ", " << ViewProjectionMatrix._23 << ", " << ViewProjectionMatrix._24;
			line = ss.str(); ss.str(std::string());
			dxDrawText(Device, 100, 470, 0xFFFFFFFF, (char*)line.c_str());
			ss <<  ViewProjectionMatrix._31 <<", "<< ViewProjectionMatrix._32 << ", "<< ViewProjectionMatrix._33 << ", " << ViewProjectionMatrix._34;
			line = ss.str(); ss.str(std::string());
			dxDrawText(Device, 100, 490, 0xFFFFFFFF, (char*)line.c_str());
			ss << ViewProjectionMatrix._41 << ", " << ViewProjectionMatrix._42 << ", " << ViewProjectionMatrix._43 << ", " << ViewProjectionMatrix._44;
			line = ss.str(); ss.str(std::string());
			dxDrawText(Device, 100, 510, 0xFFFFFFFF, (char*)line.c_str());*/
/*	if(cameraPtr)
	{
		if(hack_config.pitchCheck || hack_config.yawCheck || hack_config.rollCheck || hack_config.zoomCap>1100)
		{
			//try roll, which should default to 90 in basically all zones
			float currentRoll = *(float*)(cameraPtr + 0xB0);
			if(currentRoll < 0.0001f)
			{
				hookCamera(true);
			}
		}
	}*/
	DWORD camera = FindDMAAddy(cameraPtrBase, offsets);
	if (camera/* && hookFlag*/)
	{
		D3DXMATRIX viewMat;
		D3DXMatrixIdentity(&viewMat);
		
	//	cameraPtr = camera;
		viewMat[0] = *(float*)(camera + 0x68);
		viewMat[1] = *(float*)(camera + 0x6C);
		viewMat[2] = *(float*)(camera + 0x70);
		viewMat[3] = 0;

		viewMat[4] = *(float*)(camera - 0x48);
		viewMat[5] = *(float*)(camera - 0x44);

		viewMat[8] = *(float*)(camera + 0x28);
		viewMat[9] = *(float*)(camera + 0x2C);
		viewMat[10] = *(float*)(camera + 0x30);

		//camera + 0xF0 = camera-z
		//0xAC  roll offset outside house, in house B0
		BYTE rollOffset = 0x28;
		if (viewMat[10]<-91.0f)
		{
			//FIELD OFFSETS
			rollOffset = 0x3C;
			viewMat[8] = *(float*)(camera + rollOffset);
			viewMat[9] = *(float*)(camera + (rollOffset+0x4));
			viewMat[10] = *(float*)(camera + (rollOffset+0x8));
		}

		viewMat[12] = hack_config.zoomCap;
		viewMat[13] = hack_config.viewRoll;
		viewMat[14] = hack_config.viewYaw;
		viewMat[15] = hack_config.viewPitch;

		

		if (hack_config.zoomCap != *(float*)(camera - 0x44))
			*(float*)(camera - 0x44) = hack_config.zoomCap;
		if (hack_config.rollCheck)
			*(float*)(camera + rollOffset) = (hack_config.viewRoll);
		if (hack_config.yawCheck)
			*(float*)(camera + (rollOffset+0x4)) = hack_config.viewYaw;
		if (hack_config.pitchCheck)
			*(float*)(camera + (rollOffset+0x8)) = hack_config.viewPitch;

		if (1/*showDebug*/) {
			FillRGB(Device, 50, 390, 350, 270, D3DCOLOR_ARGB(200, 34, 34, 34));
			std::stringstream ss;
			std::string line;

			/*ss << "Hook State: " << hookFlag;
			line = ss.str(); ss.str(std::string());
			dxDrawText(dxFont, 70, 405, 0xFFFFFFFF, (char*)line.c_str());*/
			ss << std::hex << camera;
			line = ss.str(); ss.str(std::string());
			dxDrawText(dxFont, 70, 420, 0xFFFFFFFF, (char*)line.c_str());
			ss << "Eye: " << viewMat._11 << ", " << viewMat._12 << ", " << viewMat._13 << ", " << viewMat._14;
			line = ss.str(); ss.str(std::string());
			dxDrawText(dxFont, 100, 450, 0xFFFFFFFF, (char*)line.c_str());
			ss << "Zoom: " << viewMat._21 << " Max: " << viewMat._22 /*<< ", " << viewMat._23 << ", " << viewMat._24*/;
			line = ss.str(); ss.str(std::string());
			dxDrawText(dxFont, 100, 470, 0xFFFFFFFF, (char*)line.c_str());
			ss << "Roll: " << viewMat._31 << " Yaw: " << viewMat._32 << " Pitch:  " << viewMat._33 /*<< ", " << viewMat._34*/;
			line = ss.str(); ss.str(std::string());
			dxDrawText(dxFont, 100, 490, 0xFFFFFFFF, (char*)line.c_str());
			ss << viewMat._41 << ", " << viewMat._42 << ", " << viewMat._43 << ", " << viewMat._44;
			line = ss.str(); ss.str(std::string());
			dxDrawText(dxFont, 100, 510, 0xFFFFFFFF, (char*)line.c_str());
		}
	}

#ifdef angleControl
	eyex = *(float*)(cameraPtr + 0x68);
	eyey = *(float*)(cameraPtr + 0x6C);
	if (xUpdate)
		*(float*)(cameraPtr + 0x68) += eyeXOffset;
	if (yUpdate)
		*(float*)(cameraPtr + 0x6C) += eyeYOffset;

	*(float*)(cameraPtr + 0xC4) = roll;
	*(float*)(cameraPtr + 0xC8) = yaw;
	*(float*)(cameraPtr + 0xCC) = pitch;
#endif
	return oEndScene(Device);
}

HRESULT APIENTRY hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{
	if (dxFont)
		dxFont->OnLostDevice();

	HRESULT result = oReset(pDevice, pPresentationParameters);
	if (SUCCEEDED(result))
	{
		if (dxFont)
			dxFont->OnResetDevice();
		m_bCreated = false;
		FirstInit = false;
		ImGui_ImplDX9_InvalidateDeviceObjects();
		//if (line)
			//line->OnResetDevice();

		//stateOnResetDevice(pDevice);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
	return result;
}
//=====================================================================================

LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
extern IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		//		return 1;
	}
	return CallWindowProc(game_wndprc, hWnd, msg, wParam, lParam);
	//return DefWindowProc(hWnd, msg, wParam, lParam);
}
//=====================================================================================
DWORD* pVTable;
void DX_Init(DWORD* table)
{
	/*WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
	RegisterClassEx(&wc);
	HWND hWnd = CreateWindow("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);*/
	LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // Present with vsync
	d3dpp.hDeviceWindow = game_hwnd;

	pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, game_hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &Device);
	pVTable = (DWORD*)Device;
	pVTable = (DWORD*)pVTable[0];
	//	DestroyWindow(hWnd);
}

BOOL CALLBACK find_game_hwnd(HWND hwnd, LPARAM game_pid) {
	// Skip windows not belonging to the game process.
	DWORD hwnd_pid = NULL;
	GetWindowThreadProcessId(hwnd, &hwnd_pid);
	if (hwnd_pid != game_pid)
		return TRUE;
	// Set the target window handle and stop the callback.
	game_hwnd = hwnd;
	return FALSE;
}
void Log(const wchar_t *fmt, ...)
{
	va_list list;
	static wchar_t buffer[1024];
	static DWORD w;
	FILE *f;

	va_start(list, fmt);

	// To console
	DWORD len = wvsprintf(LPSTR(buffer), LPCSTR(fmt), list);
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), buffer, len, (DWORD *)&w, NULL);

/*	if (fopen_s(&f, "dinput8.log", "a+") == 0)
	{
		vfwprintf(f, fmt, list);
		fflush(f);
		fclose(f);
	}
	*/
	va_end(list);
}
void ShowMapleStoryDebugConsole()
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());

	char charConsoleName[128];
	sprintf_s(charConsoleName, "MapleStory Debug Console | PID: %08X (%d)", GetCurrentProcessId(), GetCurrentProcessId());
	SetConsoleTitleA(charConsoleName);
}
uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		addr = *(uintptr_t*)addr;
		if (!addr)
			return 0;
		addr += offsets[i];
	}
	return addr;
}
HMODULE hmRendDx9Base = NULL;
DWORD WINAPI HookThread(LPVOID)
{
	DWORD VTable[3] = { 0 };

	while (hmRendDx9Base == NULL)
	{
		hmRendDx9Base = GetModuleHandleA("d3d9.dll");
		Sleep(250);
	}
	EnumWindows(find_game_hwnd, GetCurrentProcessId());
/*	setlocale(LC_ALL, "");
	ShowMapleStoryDebugConsole();*/
	DWORD addyCameraPtr = AobScan("E8 ?? ?? ?? ?? 85 ?? 75 ?? 8B ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 ?? 5F");
	cameraPtrBase = *(DWORD*)(addyCameraPtr + 0x0B);
	//cameraPtr = *(DWORD*)(cameraPtr + 0x124+0x88);
	
	cameraPtr = FindDMAAddy(cameraPtrBase, offsets);
	//cameraPtr = *(DWORD*)(cameraPtr + 0x88);
	/*cameraPtr += 0x88;
	cameraPtr = *(DWORD*)(cameraPtr);*/

	DX_Init(VTable);
	oDrawIndexedPrimitive = (tDrawIndexedPrimitive)DetourFunction((BYTE*)pVTable[82], (BYTE*)hkDrawIndexedPrimitive);
	oEndScene = (tEndScene)DetourFunction((BYTE*)pVTable[42], (BYTE*)hkEndScene);
	oReset = (tReset)DetourFunction((BYTE*)pVTable[16], (BYTE*)hkReset);
	/*
		const char *pbPattern = "\x8B\x90\x88\x00\x00\x00\xff\x00\xB0\x00";
		char *pszMask = (char*)"xxxxxxx?x?";
		size_t aobSize = 6;
		camHookEntry = (DWORD)FindPattern((char*)pbPattern, pszMask, aobSize);
	//	camHookEntry = AobScan("8b 90 88 00 00 00 ff ?? b0 ??");
		camHookExit = camHookEntry + 0x6;
		std::stringstream ss;
		ss << camHookEntry<<std::endl;
		std::ofstream outFile("newaobscan.log", std::ios_base::app);
		outFile << ss.str() << std::endl;
		outFile.close();
	*/
	//	else
		//	camHookExit = camHookEntry + 0x5;

	return 0;
}

//=========
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ulReason, LPVOID lpReserved)
{
	switch (ulReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&HookThread, 0, 0, 0);
	}
	break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}