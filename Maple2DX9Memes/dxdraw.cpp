#include "dxdraw.h"

//ARGB color values
void dxDrawText(LPD3DXFONT dxFont, INT x, INT y, DWORD color, const char *text, DWORD flag)
{
	RECT rect;
	SetRect(&rect, x, y, x + 100, y);
	dxFont->DrawText(NULL, text, -1, &rect, flag | DT_NOCLIP, color);
}
//=====================================================================================

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
	D3DXVec4Transform(&screenCords, &D3DXVECTOR4(x, y, z, 1), &ViewProjectionMatrix);
	D3DXVec3Project(&Vector2D, &Vector3D, &Viewport, &WorldMatrix, &ProjMatrix, &ViewProjectionMatrix);
	return Vector2D;
}