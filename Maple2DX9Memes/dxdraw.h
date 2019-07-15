#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#ifndef _WIN64
void FillRGB(LPDIRECT3DDEVICE9 Device, float x, float y, float w, float h, D3DCOLOR color);
void FillRGB(LPDIRECT3DDEVICE9 Device, float x, float y, float w, float h, D3DCOLOR color);

void DrawBox(LPDIRECT3DDEVICE9 Device, float x, float y, float width, float height, float px, D3DCOLOR color);
#endif
void dxDrawText(LPD3DXFONT dxFont, INT x, INT y, DWORD color, const char *text, DWORD flag = DT_LEFT);

bool bodyPrim(UINT NumVertices, UINT primCount);