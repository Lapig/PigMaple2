#include "dxdraw.h"

//ARGB color values
void dxDrawText(LPD3DXFONT dxFont, INT x, INT y, DWORD color, const char *text, DWORD flag)
{
	RECT rect;
	SetRect(&rect, x, y, x + 100, y);
	dxFont->DrawTextA(NULL, text, -1, &rect, flag | DT_NOCLIP, color);
}
//=====================================================================================
#ifndef _WIN64
void FillRGB(LPDIRECT3DDEVICE9 Device, float x, float y, float w, float h, D3DCOLOR color)
{
	static ID3DXLine* pLine;

	if (!pLine)
		D3DXCreateLine(Device, &pLine);

	D3DXVECTOR2 vLine[2];

	pLine->SetWidth(w);
	pLine->SetAntialias(false);
	pLine->SetGLLines(true);

	vLine[0].x = x + w / 2;
	vLine[0].y = y;
	vLine[1].x = x + w / 2;
	vLine[1].y = y + h;

	pLine->Begin();
	pLine->Draw(vLine, 2, color);
	pLine->End();
}

void DrawBox(LPDIRECT3DDEVICE9 Device, float x, float y, float width, float height, float px, D3DCOLOR color)
{
	//y -= height;

	x -= width; y -= height;
	FillRGB(Device, x, y + height, width, px, color);
	FillRGB(Device, x - px, y, px, height, color);
	FillRGB(Device, x, y - px, width, px, color);
	FillRGB(Device, x + width, y, px, height, color);
}
#endif
#ifdef DEV
void SetViewSpaceMatrix(LPDIRECT3DDEVICE9 Device, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	D3DXMATRIX m_matView;
	D3DXVECTOR3 pUp(0, 1, 0);
	D3DXMatrixLookAtLH(&m_matView, pEye, pAt, &pUp);

	Device->SetTransform(D3DTS_VIEW, &m_matView);
}
D3DXVECTOR3 world2Screen(LPDIRECT3DDEVICE9 Device, float x, float y, float z)
{
	int registerNum = 0;

	D3DVIEWPORT9 Viewport;
	D3DXMATRIX ViewProjectionMatrix, WorldMatrix, WorldToLocal, LocalToWorld, ProjMatrix;
	D3DXMatrixIdentity(&LocalToWorld);
	D3DXVECTOR3 Vector3D(x, y, z), Vector2D;
	Device->GetVertexShaderConstantF(registerNum, ViewProjectionMatrix, 4);//change this to match your game matrix

//	D3DXMatrixTranspose(&ProjMatrix, &ViewProjectionMatrix);
	Device->GetVertexShaderConstantF(0, WorldMatrix, 4);//change this to match your game matrix
	Device->GetViewport(&Viewport);
	D3DXMatrixIdentity(&WorldToLocal);
	D3DXMatrixIdentity(&ProjMatrix);
	D3DXVECTOR4 screenCords;

	D3DXVECTOR4 unk(x, y, z, 1);
	D3DXVec4Transform(&screenCords, &unk, &ViewProjectionMatrix);
	D3DXVec3Project(&Vector2D, &Vector3D, &Viewport, &WorldMatrix, &ProjMatrix, &ViewProjectionMatrix);
	return Vector2D;
}
#endif
bool bodyPrim(UINT NumVertices, UINT primCount)
{
	return (NumVertices == 154 && primCount == 152) ||
		(NumVertices == 171 && primCount == 306) ||
		(NumVertices == 332 && primCount == 226) ||
		(NumVertices == 2498 && primCount == 3414) ||
		(NumVertices == 813 && primCount == 1129) ||
		(NumVertices == 693 && primCount == 978) ||
		(NumVertices == 272 && primCount == 412) ||
		(NumVertices == 1224 && primCount == 1454) ||
		(NumVertices == 256 && primCount == 302) ||
		(NumVertices == 345 && primCount == 360) ||
		(NumVertices == 286 && primCount == 282) ||
		(NumVertices == 126 && primCount == 174) ||
		(NumVertices == 230 && primCount == 336) ||
		(NumVertices == 158 && primCount == 218) ||
		(NumVertices == 310 && primCount == 486) ||
		(NumVertices == 171 && primCount == 306);
}