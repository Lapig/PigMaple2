#pragma once
#include "engineFunctions.h"
#include "Maple2DX9Memes.h"
#include "Asm.h"
#include "obfuscate.h"

using namespace Asm;

double v2;
int v3;

int a1;
DWORD* a2;

 __declspec(naked) void movement_handler(/*int a1, DWORD *a2*/)
{
	 __asm {
		 mov [a1],eax
		 mov [a2],ecx
	 }
	*(DWORD *)(a1 + 304) = *a2;
	*(DWORD *)(a1 + 308) = a2[1];
	*(DWORD *)(a1 + 312) = a2[2];
	*(DWORD *)(a1 + 316) = a2[3];
	*(DWORD *)(a1 + 320) = a2[4];
	*(DWORD *)(a1 + 324) = a2[5];
	*(DWORD *)(a1 + 280) = *a2;
	*(DWORD *)(a1 + 284) = a2[1];
	*(DWORD *)(a1 + 288) = a2[2];
	*(DWORD *)(a1 + 292) = a2[3];
	*(DWORD *)(a1 + 296) = a2[4];
	*(DWORD *)(a1 + 300) = a2[5];
	*(DWORD *)(a1 + 256) = *a2;
	*(DWORD *)(a1 + 260) = a2[1];
	*(DWORD *)(a1 + 264) = a2[2];
	*(DWORD *)(a1 + 268) = a2[3];
	*(DWORD *)(a1 + 272) = a2[4];
	*(DWORD *)(a1 + 276) = a2[5];
	 v2 = *(double *)&a2[2 * *(DWORD *)(a1 + 16)];
	 v3 = *(DWORD *)(a1 + 248);
	*(double *)(a1 + 328) = v2;
	if (v3)
	{
		float v5 = *(double *)(a1 + 256);
		float v6 = *(double *)(a1 + 264);
		float v7 = *(double *)(a1 + 272);
		(*(void(__stdcall **)(float *))(*(DWORD *)v3 + 56))(&v5);
	}
	__asm {
		mov al,1
	}
}