#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
//#pragma comment(lib, "d3dx9.lib")
//#pragma comment(lib, "d3d9.lib")
#include <detours.h>
#include <sstream>
#include <string>
#include "Asm.h"
#include "Menu/imgui.h"
#include "Maple2DX9Memes.h"
#include "Menu/settings_form.h"
#include "dxdraw.h"
#include <vector>
#include "memutil.h"


#include <TlHelp32.h>
#include <iostream>

#include "flags.h"

//#define TEST_ENV		//For gui testing, allows injection into any dx9 exe

using namespace Asm;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT(__stdcall * tDrawIndexedPrimitive)(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
tDrawIndexedPrimitive oDrawIndexedPrimitive;

typedef HRESULT(__stdcall * tEndScene)(LPDIRECT3DDEVICE9 Device);
tEndScene oEndScene;

typedef HRESULT(__stdcall * tReset)(LPDIRECT3DDEVICE9 Device, D3DPRESENT_PARAMETERS* pPresentationParameters);
tReset oReset;

typedef HRESULT(__stdcall * tSetRenderState)(LPDIRECT3DDEVICE9 Device, D3DRENDERSTATETYPE State, DWORD Value);
tSetRenderState oSetRenderState;

typedef HRESULT(__stdcall * tBeginScene)(LPDIRECT3DDEVICE9 Device);
tBeginScene oBeginScene;
typedef HRESULT(__stdcall * tDrawPrimitive)(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
tDrawPrimitive oDrawPrimitive;
typedef HRESULT(__stdcall * tPresent)(LPDIRECT3DDEVICE9 Device, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
tPresent oPresent;
typedef HRESULT(__stdcall * tSetVertexShader)(LPDIRECT3DDEVICE9 Device, IDirect3DVertexShader9* pShader);
tSetVertexShader oSetVertexShader;
typedef HRESULT(__stdcall * tUpdateTexture)(LPDIRECT3DDEVICE9 Device, IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture);
tUpdateTexture oUpdateTexture;
typedef HRESULT(__stdcall * tRelease)(LPDIRECT3DDEVICE9 Device);
tRelease oRelease;

bool write_packet_hooks();

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
IDirect3DPixelShader9 *sBlue = NULL;

IDirect3DPixelShader9 *shaderPrev=NULL;
char ShaderRed[] = "ps_2_0 def c0, 1.0, 0.0, 0.0, 1.0 mov oC0, c0 mov oDepth, c0.b";
char ShaderGreen[] = "ps_2_0 def c0, 0.0, 1.0, 0.0, 1.0 mov oC0, c0 mov oDepth, c0.b";
char ShaderBlue[] = "ps_2_0 def c0, 0.0, 0.0, 1.0, 1.0 mov oC0, c0 mov oDepth, c0.b";

ID3DXBuffer *ShaderBufferColor = NULL;
ID3DXBuffer *ShaderBufferChest = NULL;
ID3DXBuffer *ShaderBufferBlue = NULL;


WNDPROC game_wndprc;
HWND game_hwnd;

/* PE Start (0x401000) */
DWORD dwCrcStart;
/* PE End */
DWORD dwCrcEnd;		//Note this is literally just the entire region
/* PE Memory Dump */
BYTE* pCrc32;

static bool MENU_DISPLAYING = false;
//=================


static DWORD playerPtr = 0x0;
static DWORD playerPtrBase = 0x0;

static DWORD cameraPtrBase = 0x0;

static DWORD loadingPtrBase = 0x0;
static DWORD fieldLoadPtr = 0x0;

static DWORD packetRecv = 0x0;
static DWORD packetSend = 0x0;

static DWORD nxCharacter = 0x0;

//FISHING MANAGER 8b ? ? ? ? ? 89 ? ? e8 ? ? ? ? b8 3RD
std::vector<unsigned int> cameraptroffsets = { 0x120,0x88 };
std::vector<unsigned int> playerbaseoffsets = { 0x1B4,0x390 };
std::vector<unsigned int> deltaspeedoffsets = { 0x1B4,0x120 };
std::vector<unsigned int> jumpheightoffsets = { 0x1B4,0x440 };

std::vector<unsigned int> risespeedoffsets = { 0x1B4, 0xe4, 0x14, 0xa8 };
std::vector<unsigned int> bobflyoffsets = { 0x1B4, 0xe4, 0x14, 0xb4 };
std::vector<unsigned int> risingtimeoffsets = { 0x1B4, 0xe4, 0x14, 0x8c };


std::vector<unsigned int> noclipoffsets = { 0x1B4, 0xe8, 0x4, 0xe8 };
std::vector<unsigned int> nofalloffsets = { 0x1B4, 0x648, 0xe8, 0x94 };
std::vector<unsigned int> nofallaltoffsets = { 0x1B4, 0x648, 0xE8, 0x98 };

std::vector<unsigned int> groundmountoffsets = { 0x1B4,0x490 };
std::vector<unsigned int> flyingspeedoffsets = { 0x1B4, 0xE4, 0x14, 0xAC };
std::vector<unsigned int> posbaseoffsets = { 0x1B4, 0xE8, 0x4, 0x10C };

std::vector<unsigned int> playerflooroffsets = { 0x1B4, 0xE8, 0x4, 0x168 };
std::vector<unsigned int> wallwalkoffsets = { 0x1B4, 0xE8, 0x4, 0x24 };

std::vector<unsigned int> playerstateoffsets = { 0x1B4, 0x648, 0xE4, 0x10 };
std::vector<unsigned int> playerstateprevoffsets = { 0x1B4, 0x648, 0xE4, 0xC };
std::vector<unsigned int> playeranimationoffsets = { 0x1B4, 0x40, 0x40, 0x20 };

static DWORD floorToggle = 0x0;
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

#if 0
MODULEENTRY32 GetModuleInfo(std::uint32_t ProcessID, const char* ModuleName)
{
	void* hSnap = nullptr;
	MODULEENTRY32 Mod32 = { 0 };

	if ((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID)) == INVALID_HANDLE_VALUE)
		return Mod32;

	Mod32.dwSize = sizeof(MODULEENTRY32);
	while (Module32Next(hSnap, &Mod32))
	{
		if (!_stricmp(ModuleName, Mod32.szModule))
		{
			CloseHandle(hSnap);
			return Mod32;
		}
	}

	CloseHandle(hSnap);
	return { 0 };
}
#endif

void registerMove() {
	DWORD statePtr = readPointerOffset(playerPtrBase, playerstateoffsets);
	DWORD prevStatePtr = readPointerOffset(playerPtrBase, playerstateprevoffsets);
	DWORD playerAnimationPtr = readPointerOffset(playerPtrBase, playeranimationoffsets);

	if (statePtr && prevStatePtr && playerAnimationPtr) {
		*(BYTE*)(statePtr) = 30;				//jumping, 3 = crawling
		*(BYTE*)(prevStatePtr) = 6;
		*(BYTE*)(playerAnimationPtr) = 157;	//balloon mount
	}
}
void teleport(float  cords[], bool target)
{
	DWORD posbase = readPointerOffset(playerPtrBase, posbaseoffsets);
//	DWORD riseDuration = readPointerOffset(playerPtrBase, risingtimeoffsets);
//	if (riseDuration && target) {
	//	registerMove();
	//	*(float*)(riseDuration) = 999;
//	}
	if (posbase && (posbase + 0x8) && (posbase + 0x10)) {
		*(float*)(posbase) = cords[0];
		*(float*)(posbase+18) = cords[0];
		*(float*)(posbase+8) = cords[1];
		*(float*)(posbase + 8+18) = cords[1];
		*(float*)(posbase+16) = cords[2];
		*(float*)(posbase + 8+8+18) = cords[2];
	}
	//registerMove();
}


void steppedTele(float cords[]) 
{
	DWORD posbase = readPointerOffset(playerPtrBase, posbaseoffsets);
	D3DXVECTOR3 currPos(getpos()), targetPos(cords);
	D3DXVECTOR3 direction = targetPos - currPos;
	const int steps = 50;
	direction /= steps;
	for (int i = 0; i < steps; i++) {
		currPos = currPos + direction;
		teleport(currPos);
		registerMove();
	}
}
float* getpos()
{
#ifdef TEST_ENV
	float test[3]={ 1,1,1 };
	return test;
#endif
	float *cords = new float[3];
	DWORD posbase = readPointerOffset(playerPtrBase, posbaseoffsets);
	if (posbase && (posbase + 0x8) && (posbase + 0x10)) {
		cords[0] = *(float*)(posbase) ;
		cords[1] = *(float*)(posbase + 8);
		cords[2] = *(float*)(posbase + 16);
	}
	return cords;
}

/*bool fieldLoaded() {
	
	BYTE testLoadByte = *(BYTE*)(fieldLoadPtr+3);
	BYTE bytes[9];
	std::string s("Byte: " );

	for (int i = 0; i < 9; i++) {
		s += std::to_string(*(BYTE*)(fieldLoadPtr + i));
		s += ", ";
	}
	dxDrawText(dxFont, 70, 420, 0xFFFFFFFF, (char*)(s).c_str());

	if (testLoadByte==100)
		return true;
	return false;
}*/
/*static bool fogReadyToClear = true;
void clearFog()
{
	fogReadyToClear = false;
//	Sleep(125);
	fogReadyToClear = true;
}
*/
bool validPlayerPtr() {
	DWORD ptr = readDWORD(playerPtrBase, 0x1b4);
	if (ptr == NULL || ptr == 0)
		return false;
	return true;
}

void resetWallStep() {
	DWORD player = readPointerOffset(playerPtrBase, wallwalkoffsets);
	if (validPlayerPtr() && player)
	{
		*(float*)(player) = 0.31f;
	}
}
void resetFloorOffset() {
	DWORD player = readPointerOffset(playerPtrBase, playerflooroffsets);
	if (validPlayerPtr() && player)
	{
		*(float*)(player) = 0.25f;
	}
}

#pragma region MACRO
typedef struct tag_KEYDATA {
	WORD keyRepeatCnt; // 16 bits
	unsigned int keyScanCode; // 8 bits
	// additional keys on the enhanced keyboard he extended keys consist of the ALT and CTRL keys on the right-hand side of the keyboard;
	// the INS, DEL, HOME, END, PAGE UP, PAGE DOWN, and arrow keys in the clusters to the left of the numeric keypad;
	// the NUM LOCK key; the BREAK (CTRL+PAUSE) key; the PRINT SCRN key; and the divide (/) and ENTER keys in the numeric keypad.
	BYTE flagExtendedKey : 1;
	BYTE Reserved : 3;
	BYTE flagAltDown : 1;
	BYTE flagRepeat : 1;
	BYTE flagUp : 1;
} KEYDATA;
DWORD createKeyData(int Key) {
	KEYDATA kd;
	memset(&kd, 0, sizeof(KEYDATA));
	kd.keyRepeatCnt = 1;
	kd.keyScanCode = MapVirtualKey(Key, 0);
	kd.flagExtendedKey = 0;
	kd.Reserved = 0;
	kd.flagAltDown = 0; // this has to be 0 else no lumping of messages
	kd.flagRepeat = 1; // this is necessary for proper execution in maplestory
	kd.flagUp = 0;
	DWORD dwVal;
	memcpy(&dwVal, &kd, sizeof(DWORD));
	return dwVal;
}
static unsigned int prevKey = 0;
void key_press(unsigned int key, bool keyup)
{
	//PostMessage(game_hwnd, (keyup) ? WM_KEYUP : WM_KEYDOWN, MapVirtualKey(key, 0), createKeyData(key));
	//return;
//	if (::GetForegroundWindow() == game_hwnd)
//		return;
	prevKey = key;
	INPUT in;
	ZeroMemory(&in, sizeof(INPUT));
	in.type = INPUT_KEYBOARD;
	auto scanCode = MapVirtualKey(key, 0);
	
	in.ki.wVk = key;
	in.ki.wScan = scanCode; 
//	in.ki.dwFlags = (keyup) ? KEYEVENTF_KEYUP : 0;//KEYEVENTF_SCANCODE;//keydown	keyup = 0x0002
	if (keyup)
		in.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;//keydown	keyup = 0x0002
	else
		in.ki.dwFlags = 0;
	in.ki.time = 0;
	in.ki.dwExtraInfo = NULL;

	SendInput(1, &in, sizeof(INPUT));
	//PostMessage(game_hwnd, WM_KEYDOWN, key, NULL);
	//PostMessage(game_hwnd, WM_CHAR, key, 1);
	//PostMessage(game_hwnd, WM_KEYUP, key, NULL);

}
#pragma endregion MACRO

#pragma region NULLRENDER
HRESULT hkPresent(LPDIRECT3DDEVICE9 Device, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	if (hack_config.nullRender) {
		Sleep(15);
		return D3D_OK;
	}
	return oPresent(Device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
HRESULT hkRelease(LPDIRECT3DDEVICE9 Device)
{
	return D3D_OK;
}
HRESULT hkSetVertexShader(LPDIRECT3DDEVICE9 Device, IDirect3DVertexShader9* pShader)
{
	if (hack_config.nullRender)
		return oSetVertexShader(Device, NULL);;
	return oSetVertexShader(Device, pShader);
}
HRESULT hkUpdateTexture(LPDIRECT3DDEVICE9 Device, IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
	if (hack_config.nullRender)
		return D3D_OK;
	return oUpdateTexture(Device, pSourceTexture, pDestinationTexture);
}
HRESULT __stdcall hkDrawPrimitive(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	if (hack_config.nullRender)
		return D3D_OK;
	return oDrawPrimitive(Device, PrimitiveType, StartVertex, PrimitiveCount);
}
HRESULT __stdcall hkBeginScene(LPDIRECT3DDEVICE9 Device) 
{
	if (hack_config.nullRender) {
		oBeginScene(Device);
		Device->Present(NULL, NULL, NULL, NULL);
		return D3D_OK;
	}
	return oBeginScene(Device);
}
HRESULT __stdcall hkSetRenderState(LPDIRECT3DDEVICE9 Device, D3DRENDERSTATETYPE State, DWORD Value)
{
	float fog = 0.0f;

	
	if (hack_config.setRenderLog) {
		/*if (State == D3DRS_FOGENABLE && hack_config.noFog) {
			Value = 0;
		}
		else if (State == D3DRS_FOGDENSITY && hack_config.noFog) {
			Value = *((DWORD*)(&fog));
		}
		if (State > 27 && State < 52 && State != 29 && renders.size() < 100)*/
			renders.push_back(renderChange(State, Value));
	}
	if (hack_config.noFog && (State== D3DRS_FOGENABLE || State == D3DRS_FOGDENSITY || State == D3DRS_FOGEND || State == D3DRS_FOGSTART)) {
		return D3D_OK;
		//return oSetRenderState(Device, State, *((DWORD*)(&fog)));
	}
	return oSetRenderState(Device, State, Value);
}
#pragma endregion NULLRENDER
static int keydownFrameCount = 0;
HRESULT __stdcall hkDrawIndexedPrimitive(LPDIRECT3DDEVICE9 Device, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	if (Device == nullptr)
		return oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

	if (FirstInit == FALSE)
	{
		D3DXAssembleShader(ShaderGreen, sizeof(ShaderGreen), NULL, NULL, 0, &ShaderBufferColor, NULL);
		Device->CreatePixelShader((const DWORD*)ShaderBufferColor->GetBufferPointer(), &sGreen);
		ShaderBufferColor->Release();

		D3DXAssembleShader(ShaderRed, sizeof(ShaderRed), NULL, NULL, 0, &ShaderBufferChest, NULL);
		Device->CreatePixelShader((const DWORD*)ShaderBufferChest->GetBufferPointer(), &sRed);
		ShaderBufferChest->Release();

		D3DXAssembleShader(ShaderBlue, sizeof(ShaderBlue), NULL, NULL, 0, &ShaderBufferBlue, NULL);
		Device->CreatePixelShader((const DWORD*)ShaderBufferBlue->GetBufferPointer(), &sBlue);
		ShaderBufferBlue->Release();
	//	fieldLoadPtr = AobScan("5a 96 ?? ?? 5a 96 90 01");

//Currently useless but neat

#if defined(TEST_ENV)==0 && defined(DEV)
		dwCrcStart = reinterpret_cast<unsigned int>(GetModuleHandleA("MapleStory2.exe"));

		if (dwCrcStart != 0) {
			DWORD dwCrcSize = PIMAGE_NT_HEADERS(dwCrcStart + PIMAGE_DOS_HEADER(dwCrcStart)->e_lfanew)->OptionalHeader.SizeOfImage;

			pCrc32 = reinterpret_cast<unsigned char*>(malloc(dwCrcSize));
			memcpy(pCrc32, reinterpret_cast<void*>(dwCrcStart), dwCrcSize);

			dwCrcEnd = dwCrcStart + dwCrcSize;
		}
		else {
			Asm::ErrorMessage("CRC Error");
		}

	//	write_packet_hooks();
#endif

		FirstInit = TRUE;
	}

	DWORD imguiFlag = 0x0;
	Device->GetRenderState(D3DRS_STENCILFAIL, &imguiFlag);	//6 indicates imgui call

	if (imguiFlag == 6)
		return oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

	if (hack_config.nullRender)
		return D3D_OK;
	
	//get stride
	LPDIRECT3DVERTEXBUFFER9 StreamData;
	UINT OffsetInBytes;
	UINT Stride;
	Device->GetStreamSource(0, &StreamData, &OffsetInBytes, &Stride);// == D3D_OK;
	{
		if (StreamData != NULL)
			StreamData->Release();
	}
	if (hack_config.playerChams && bodyPrim(NumVertices, primCount)) {
	//	Device->GetPixelShader(&shaderPrev);
		Device->SetPixelShader(sBlue);
		return D3D_OK;
	//	return oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	/*	oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		if (shaderPrev != NULL) {
			Device->SetPixelShader(shaderPrev);
			//shaderPrev->Release();
		}
		return D3D_OK;*/
	}
/*
	//get vSize
	HRESULT hRet = Device->GetVertexShader(&vShader);
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

/*	if ((Stride == 20 || Stride == 16))//text? chat at least
	{
		return oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}*/

//	Device->GetRenderState(D3DRS_ZENABLE, &zEnable);

	if (hack_config.portalChams && (NumVertices == 144 && primCount == 280)) {	//portals
		Device->SetPixelShader(sGreen);
	}
	else if (hack_config.chestChams && ((NumVertices == 291 && primCount == 424) || (NumVertices == 125 && primCount == 188)) )
	{
	//	Device->SetRenderState(D3DRS_ZENABLE, false);
		Device->GetPixelShader(&shaderPrev);
		Device->SetPixelShader(sRed);
		oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		if (shaderPrev != NULL) {
			Device->SetPixelShader(shaderPrev);
		//	shaderPrev->Release();
		}
		
		if (hack_config.setPrimLog && renders.size()<100) {
			renders.push_back(renderChange((D3DRENDERSTATETYPE)Stride, startIndex));
		}
		return D3D_OK;
	}
//	Device->SetRenderState(D3DRS_ZENABLE, zEnable);

//	Device->SetRenderState(D3DRS_FOGENABLE, false);
	//Device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
	return oDrawIndexedPrimitive(Device, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

//=====================================================================================
float pointx = 0, pointy = 0;
DWORD addyPlayerBasePtr;

float prevZPos = 0;
HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 Device)
{
	if (Device == nullptr)
		return oEndScene(Device);
	if (!m_bCreated)
	{
		m_bCreated = true;
		D3DXCreateFontA(Device, 14, 0, FW_BOLD, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas", &dxFont);

		Device->GetViewport(&viewport);

		D3DDEVICE_CREATION_PARAMETERS CParams;
		Device->GetCreationParameters(&CParams);
		game_wndprc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(CParams.hFocusWindow, GWLP_WNDPROC, (LONG_PTR)WndProc));

		menu_init(game_hwnd, Device);
	}

//	DWORD _play = readPointerOffset(playerPtrBase, playerbaseoffsets);
	if ((GetAsyncKeyState(0x2D) & 1)) {
		MENU_DISPLAYING = !MENU_DISPLAYING;
	}
	if (MENU_DISPLAYING)
	{
		//insert
		draw_menu(&MENU_DISPLAYING, 0);
	}

	if ((GetAsyncKeyState(0x2E) & 1) && hack_config.nullRenderCheck) {//del
		hack_config.nullRender = !hack_config.nullRender;
	}
	if (hack_config.nullRender)
		return D3D_OK;
	if (hack_config.holdKey) {
		int boxw = 20;
		FillRGB(Device, (viewport.Width / 2)-(boxw /2), viewport.Height - 140, boxw, 20, D3DCOLOR_ARGB(255, 0, 255, 0));

		 if (keydownFrameCount==0) {
			key_press(prevKey);
		}
	//	if (GetAsyncKeyState(prevKey))
		{
			if (keydownFrameCount < 100)
				keydownFrameCount++;
			else {
				keydownFrameCount = -10;
				//key_press(prevKey);
				key_press(prevKey, true);
			}
		}
		
	}

	if (GetAsyncKeyState(0x70) & 1) {
		hack_config.holdKey = !hack_config.holdKey;
		if (hack_config.holdKey)
			key_press(hack_config.keycode);
	}
	
#ifndef TEST_ENV
		DWORD camera = readPointerOffset(cameraPtrBase, cameraptroffsets);
		if (camera && validPlayerPtr())
		{
			D3DXMATRIX viewMat;
			D3DXMatrixIdentity(&viewMat);

			//	cameraPtr = camera;
		/*	viewMat[0] = *(float*)(camera - 0x20); //eye x		//HOUSE 
			viewMat[1] = *(float*)(camera - 0x1C); //eye y
			viewMat[2] = *(float*)(camera - 0x18); //eye z		*/
			viewMat[0] = *(float*)(camera + 0x30); //eye x		FIELD
			viewMat[1] = *(float*)(camera + 0x34); //eye y
			viewMat[2] = *(float*)(camera + 0x38); //eye z
			viewMat[3] = 0;

			viewMat[4] = readFloat(camera ,-0x48);
			viewMat[5] = *(float*)(camera - 0x44);
			viewMat[6] = *(float*)(camera - 0x48); //FOG

			viewMat[8] = *(float*)(camera + 0x28);
			viewMat[9] = *(float*)(camera + 0x2C);
			viewMat[10] = *(float*)(camera + 0x30);


			//camera + 0xF0 = camera-z
			//0xAC  roll offset outside house, in house B0
			BYTE rollOffset = 0x28;
			if (viewMat[10] < -91.0f)
			{
				//FIELD OFFSETS
				rollOffset = 0x3C;
				viewMat[8] = *(float*)(camera + rollOffset);
				viewMat[9] = *(float*)(camera + (rollOffset + 0x4));
				viewMat[10] = *(float*)(camera + (rollOffset + 0x8));
			}

			viewMat[12] = (float)hack_config.zoomCap;
			viewMat[13] = hack_config.viewRoll;
			viewMat[14] = hack_config.viewYaw;
			viewMat[15] = hack_config.viewPitch;

			DWORD _play = readPointerOffset(playerPtrBase, playerbaseoffsets);

//			if(fogReadyToClear)
//				*(float*)(camera - 0x48) = 20000.f;
			if (hack_config.zoomCap != readFloat(camera, -0x44))
				*(float*)(camera - 0x44) = (float)hack_config.zoomCap;
			if (hack_config.rollCheck)
				*(float*)(camera + rollOffset) = (hack_config.viewRoll);
			if (hack_config.yawCheck)
				*(float*)(camera + (rollOffset + 0x4)) = hack_config.viewYaw;
			if (hack_config.pitchCheck)
				*(float*)(camera + (rollOffset + 0x8)) = hack_config.viewPitch;
#ifdef DEV
			if (hack_config.eyeXCheck) {
				*(float*)(camera + 0x30) = hack_config.eyeX;
			}
			else {
				hack_config.eyeX = viewMat[0];
				hack_config.eyeXPrev = hack_config.eyeX;
			}
			if (hack_config.eyeYCheck) {
				*(float*)(camera + 0x34) = hack_config.eyeY;
			}
			else {
				hack_config.eyeY = viewMat[1];
				hack_config.eyeYPrev = hack_config.eyeY;
			}
#endif
			if (validPlayerPtr() && _play) {
#ifdef BANABLE
				if (player_hacks.moveToggle && *(int*)(_play + 0x70) != player_hacks.moveSpeed  && *(int*)(_play + 0x70) < 200)
					*(int*)(_play + 0x70) = player_hacks.moveSpeed;
				if (player_hacks.flightSpeedToggle) {
					DWORD flightSpeedPtr = readPointerOffset(playerPtrBase, flyingspeedoffsets);
					if (flightSpeedPtr /*&& *(float*)(flightSpeedPtr) != player_hacks.flyingSpeed && readFloat(flightSpeedPtr) < 30*/) {

						*(float*)(flightSpeedPtr) = player_hacks.flyingSpeed;
					}
				}
				if (player_hacks.groundMountToggle) {
					DWORD mountSpeedPtr = readPointerOffset(playerPtrBase, groundmountoffsets);
					*(float*)(mountSpeedPtr) = player_hacks.groundMountSpeed;
				}
#endif
				if (player_hacks.deltaToggle) {
					DWORD deltaspeedptr = readPointerOffset(playerPtrBase, deltaspeedoffsets);
					if (deltaspeedptr && *(float*)(deltaspeedptr) != player_hacks.deltaSpeed) {
						*(float*)(deltaspeedptr) = player_hacks.deltaSpeed;
					}
				}
				else {
					DWORD deltaspeedptr = readPointerOffset(playerPtrBase, deltaspeedoffsets);
					if (deltaspeedptr)
						*(float*)(deltaspeedptr) = 3.5;
				}
				if (player_hacks.noFall) {
					DWORD nofallptr = readPointerOffset(playerPtrBase, nofalloffsets);
					if (nofallptr) {
						*(float*)(nofallptr) = 0;
						
						if (player_hacks.prevZ) {
							float *z = getpos();
							float modz = player_hacks.prevZ;
							if (z[2] > modz) {
								modz = z[2];
								player_hacks.prevZ = z[2];
							}
							float newz[3] = { z[0],z[1],modz };
							teleport(newz, false);
						}
					}
				}
				if (player_hacks.floorOffsetToggle) {
					DWORD floorptr = readPointerOffset(playerPtrBase, playerflooroffsets);
					if (floorptr)
					{
						*(float*)(floorptr) = (float)(player_hacks.floorOffset);
					}
				}
				if (player_hacks.noClip) {
					DWORD noclipptr = readPointerOffset(playerPtrBase, noclipoffsets);
					if (noclipptr)
						*(float*)(noclipptr) = 0;
				}
				else if (player_hacks.semiNoClip) {
					DWORD noclipptr = readPointerOffset(playerPtrBase, noclipoffsets);
					if (noclipptr)
						*(float*)(noclipptr) = 0.8f;
				}
				else if (player_hacks.fMountNoClip) {
					DWORD state = readPointerOffset(playerPtrBase, playerstateoffsets);
					DWORD noclipptr = readPointerOffset(playerPtrBase, noclipoffsets);

					if (noclipptr && state && *(BYTE*)(state) == 30) {//flying
						*(float*)(noclipptr) = 0;
					}
					else if(noclipptr){
						*(float*)(noclipptr) = 1.5;
					}
				}
				else {
					DWORD noclipptr = readPointerOffset(playerPtrBase, noclipoffsets);
					if (noclipptr && *(float*)(noclipptr) != 1.5)
						*(float*)(noclipptr) = 1.5;
				}
				if (player_hacks.turboRise) {
					DWORD speed = readPointerOffset(playerPtrBase, risespeedoffsets);
					DWORD bob = readPointerOffset(playerPtrBase, bobflyoffsets);
					if (speed && bob && *(float*)(speed)>0.1) {
						*(float*)(speed) = 8.0f;
						*(float*)(bob) = 0.0f;
					}
				}
				if (player_hacks.posBypass)
				{
					DWORD state = readPointerOffset(playerPtrBase, playerstateoffsets);
					DWORD prevState = readPointerOffset(playerPtrBase, playerstateprevoffsets);
					*(BYTE*)(state) = 0x5;
					//*(BYTE*)(prevState) = 0x5;
				}
				if (player_hacks.floorAdjustToggle)
				{
					DWORD nofallptr = readPointerOffset(playerPtrBase, nofalloffsets);
					DWORD nofall = readPointerOffset(playerPtrBase, nofallaltoffsets);
					if (player_hacks.newFloor) {
						*(float*)(nofallptr) = 0;

						if (player_hacks.newFloor) {
							float *z = getpos();
							float modz = player_hacks.newFloor;

							float newz[3] = { z[0],z[1],modz };
							teleport(newz, false);
						}
					}
				}
				else {
					floorToggle = 0x0;
				}
				/*if (player_hacks.mountLock) {
					DWORD state = readPointerOffset(playerPtrBase, playerstateoffsets);
					DWORD prevState = readPointerOffset(playerPtrBase, playerstateprevoffsets);

					if (state && *(BYTE*)(state)!=30 && prevState && *(BYTE*)(prevState)!=30) {
						*(BYTE*)(state) = 0x1E;	//30 - flying mounts
					}
				}*/
				if (player_hacks.wallWalkToggle) {
					DWORD wallwalkPtr = readPointerOffset(playerPtrBase, wallwalkoffsets);
					if (wallwalkPtr) {
						*(float*)(wallwalkPtr) = player_hacks.wallWalkHeight;
					}
				}

		/*		if (teleQueue.size() > 0) {
					teleport(teleQueue.back(), true);
					teleQueue.pop_back();
				}*/
			}///endif player
			/*
//////debug misc
			if (hack_config.debugMode) {
				FillRGB(Device, 50, 390, 350, 270, D3DCOLOR_ARGB(200, 34, 34, 34));
				std::stringstream ss;
				std::string line;

				DWORD posbase = readPointerOffset(playerPtrBase, posbaseoffsets);

				ss << std::hex << camera;
				line = ss.str(); ss.str(std::string());
				dxDrawText(dxFont, 70, 420, 0xFFFFFFFF, (char*)line.c_str());
				ss << "Eye: " << viewMat._11 << ", " << viewMat._12 << ", " << viewMat._13 << ", " << viewMat._14;
				line = ss.str(); ss.str(std::string());
				dxDrawText(dxFont, 100, 450, 0xFFFFFFFF, (char*)line.c_str());
				ss << "Zoom: " << viewMat._21 << " Max: " << viewMat._22 /*<< ", " << viewMat._23 << ", " << viewMat._24*/;
		/*		line = ss.str(); ss.str(std::string());
				dxDrawText(dxFont, 100, 470, 0xFFFFFFFF, (char*)line.c_str());
				ss << "Roll: " << viewMat._31 << " Yaw: " << viewMat._32 << " Pitch:  " << viewMat._33 /*<< ", " << viewMat._34*/;
		/*		line = ss.str(); ss.str(std::string());
				dxDrawText(dxFont, 100, 490, 0xFFFFFFFF, (char*)line.c_str());
				ss << viewMat._41 << ", " << viewMat._42 << ", " << viewMat._43 << ", " << viewMat._44;
				line = ss.str(); ss.str(std::string());
				dxDrawText(dxFont, 100, 510, 0xFFFFFFFF, (char*)line.c_str());
			}*/
		}
#endif //testenv

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
		pDevice->GetViewport(&viewport);
		//if (line)
			//line->OnResetDevice();

		//stateOnResetDevice(pDevice);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
	return result;
}
//=====================================================================================

__declspec(naked) void movement_handler_hook()
{
	__asm {
		push ebp
		mov ebp, esp
		sub esp,0xc
//		push eax
//		push ecx
////		jmp movement_handler
//		leave
		mov esp, ebp
		add esp,0xc
		pop ebp
		ret
	}
}
#if defined(DEV) and not defined(KMS)
std::string GetClientHeader(WORD wHeader) {
	
	std::string ret = tClient[wHeader];
	return ret.empty() ? "Unknown" : ret;
}
std::string GetServerHeader(WORD wHeader) {
	std::string ret = tServer[wHeader];
	return ret.empty() ? "Unknown" : ret;
}
//_ecx = AVCOutPacket::AVCOutputStream
void __stdcall sendPacketCallback(DWORD _ecx, DWORD _retn) {
	DWORD nSize = Read<DWORD>(_ecx + 0x8);
	DWORD headerPtr =(Read<DWORD>(_ecx + 0x14));
	WORD wHeader = 0x0;
	if (headerPtr) {
		wHeader = Read<WORD>(headerPtr);
		if (wHeader == 0x12 || wHeader == 0xB)
			wHeader = 0x200;
		else if (wHeader >= 5)
			wHeader -= 5;
	}
	std::string sHeader = GetClientHeader(wHeader);
	char msg[124];
	sprintf_s(msg, 124, "SEND [%04X] %p - %i - %s - %p", wHeader, _retn, nSize, sHeader.c_str(), _ecx);
	if (wHeader != 0x200) {
		if (sentPackets.size() > 0) {
			if (strncmp(sentPackets.back().c_str(), msg, 12) == 0)
				return;
		}
		sentPackets.push_back(std::string(msg));
	}
	if (wHeader == 0x1B) {
		if (validPlayerPtr()) {
			DWORD speed = readPointerOffset(playerPtrBase, risespeedoffsets);
			DWORD bob = readPointerOffset(playerPtrBase, bobflyoffsets);
			if (speed && bob && *(float*)(speed) > 0.1) {
				*(float*)(speed) = 8.0f;
				*(float*)(bob) = 0.0f;
			}
		}
	}
}
void __stdcall recvPacketCallback(DWORD _ecx) {
	DWORD nPad = Read<DWORD>(_ecx + 0x30);
	LPBYTE pBuffer = Read<LPBYTE>(_ecx + 0x10) + nPad;
	DWORD nSize =Read<DWORD>(_ecx + 0x20);//- nPad;
	WORD wHeader =Read<WORD>((DWORD)pBuffer);
	if (wHeader) {
		if (wHeader >= 5)
			wHeader -= 5;
	}
	std::string sHeader = GetServerHeader(wHeader);
	char msg[124];
	sprintf_s(msg, 124, "RECV [%04X] - %i - %s - %p", wHeader, nSize, sHeader.c_str(), _ecx);
	if (wHeader != 0x54 && wHeader!=0x17) {
		if (recvPackets.size() > 0) {
			//if (strncmp(recvPackets.back().c_str(), msg, 12) == 0)
//				return;
		}
		recvPackets.push_back(std::string(msg));
	}
	if (wHeader==0x18) {	//proxy game obj 7b	chat 0x18
		/*if (teleQueue.size() > 0) {
			teleport(teleQueue.back());
			teleQueue.pop_back();
		}*/
	}
}

#define ASM_RET(var,offset) _asm push dword ptr ds:[var] \
    _asm add dword ptr ds:[esp],offset \
    _asm retn
void __declspec(naked) SendPacketHook() {
	_asm {
		mov eax, [esp]
		pushad
		push eax
		push ecx
		call sendPacketCallback
		popad

		mov eax, ecx
		mov ecx, dword ptr ds : [0x01AA7C88]
		ASM_RET(packetSend, 8)
	}
}
void __declspec(naked) RecvPacketHook() {
	_asm {
		pushad
		push ebx
		call recvPacketCallback
		popad

		pop esi
		pop ebp
		ret 8
	}
}
bool write_packet_hooks() {
	initMaps();

	//	packetRecv = AobScan(AY_OBFUSCATE("8B ?? ?? 56 8B ?? ?? 2B ?? 56 51 8B 0D ?? ?? ?? ?? 03 ?? 50 E8 ?? ?? ?? ?? 5E 5D C2"));
	//	packetRecv += 19;
	DWORD packetRecv = 0x005C3476;
	// = AobScan(AY_OBFUSCATE("8B ?? 8B 0D ?? ?? ?? ?? 85 ?? 74 ?? 50 E8 ?? ?? ?? ?? C3"));
	packetSend = 0x005C3650;

	writeJmpHook(packetSend, SendPacketHook, 3);
	writeJmpHook(packetRecv, RecvPacketHook, 3);

	return true;
}
#endif //dev
LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
extern IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)==2)
	{
		if (switchTabs < 3)
			switchTabs++;
		else
			switchTabs = 0;
		return true;
	}
	/*if (msg == WM_KILLFOCUS) {
		return 1;
	}
	if (msg == WM_NCACTIVATE) {
		if (!wParam) {
			return true;
		}
	}*/
	return CallWindowProc(game_wndprc, hWnd, msg, wParam, lParam);
}
//=====================================================================================
DWORD* pVTable;
void DX_Init(DWORD* table)
{
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


#define jmp(frm, to) (int)(((int)to - (int)frm) - 5);
HMODULE hmRendDx9Base = NULL;
DWORD WINAPI HookThread(LPVOID)
{
	DWORD VTable[3] = { 0 };

	while (hmRendDx9Base == NULL)
	{
		hmRendDx9Base = GetModuleHandleA("d3d9.dll");
	}
#ifndef TEST_ENV
	while (!FindWindowA("MapleStory2", NULL));	//Dont hook cef
#endif
	EnumWindows(find_game_hwnd, GetCurrentProcessId());

#ifndef TEST_ENV
	DWORD addyCameraPtr = AobScan(AY_OBFUSCATE("E8 ?? ?? ?? ?? 85 ?? 75 ?? 8B ?? ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 ?? 5F"));
	addyPlayerBasePtr = AobScan(AY_OBFUSCATE("A1 ?? ?? ?? ?? 8B ?? ?? ?? ?? ?? 83 ?? ?? 85 ?? 74 ?? 8B ?? ?? F3 ?? ?? ?? ?? 89"));
//	loadingPtrBase = AobScan(AY_OBFUSCATE("B9 ?? ?? ?? ?? C7 ?? ?? 00 00 00 00 E8 ?? ?? ?? ?? 8B ?? ?? C7"));
	if (!addyCameraPtr || !addyPlayerBasePtr || !game_hwnd) {
		Asm::ErrorMessage("Initilization Error");
		return 1;
	}
	cameraPtrBase = *(DWORD*)(addyCameraPtr + 0x0B);
	playerPtrBase = *(DWORD*)(addyPlayerBasePtr + 0x01);
	if (!cameraPtrBase || !playerPtrBase) {
		Asm::ErrorMessage("Initilization Error");
		return 1;
	}


	playerPtr = readPointerOffset(playerPtrBase, playerbaseoffsets);

#endif
	DX_Init(VTable);
	oDrawIndexedPrimitive = (tDrawIndexedPrimitive)DetourFunction((BYTE*)pVTable[82], (BYTE*)hkDrawIndexedPrimitive);
	oEndScene = (tEndScene)DetourFunction((BYTE*)pVTable[42], (BYTE*)hkEndScene);
	oReset = (tReset)DetourFunction((BYTE*)pVTable[16], (BYTE*)hkReset);
	oBeginScene =(tBeginScene)DetourFunction((BYTE*)pVTable[41], (BYTE*)hkBeginScene);
	oDrawPrimitive = (tDrawPrimitive)DetourFunction((BYTE*)pVTable[81], (BYTE*)hkDrawPrimitive);
//	oPresent = (tPresent)DetourFunction((BYTE*)pVTable[17], (BYTE*)hkPresent);
//	oSetVertexShader = (tSetVertexShader)DetourFunction((BYTE*)pVTable[92], (BYTE*)hkSetVertexShader);
//	oUpdateTexture = (tUpdateTexture)DetourFunction((BYTE*)pVTable[31], (BYTE*)hkUpdateTexture);
//	oRelease = (tRelease)DetourFunction((BYTE*)pVTable[2], (BYTE*)hkRelease);
#ifdef TEST_ENV
	return 0;
#endif


#if defined(DEV) and not defined(KMS)
	oSetRenderState = (tSetRenderState)DetourFunction((BYTE*)pVTable[57], (BYTE*)hkSetRenderState);
	hack_config.zoomCap = 2200;
//	write_packet_hooks();
	
#endif
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
		//HookThread(NULL);

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&HookThread, 0, 0, 0);
	}
	break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
