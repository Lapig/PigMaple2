#pragma once
#include <windows.h>
#include <vector>
#include "memutil.h"
#include "flags.h"
#include <detours.h>
uintptr_t readPointerOffset(uintptr_t ptr, std::vector<unsigned int> offsets)
{
//#ifdef KMS
	if (offsets.size() > 0) {
		if (offsets[0] == 0x1B4)
			offsets[0] = 0x1B0;
		else if (offsets[0] == 0x124)
			offsets[0] = 0x128;
	}
//#endif
	uintptr_t addr = ptr;
	if (addr == 0)
		return 0;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		DWORD  old;
		VirtualProtect((PVOID)(addr), sizeof(uintptr_t), PAGE_EXECUTE_READWRITE, &old);
		addr = *(uintptr_t*)addr;
		VirtualProtect((PVOID)(addr), sizeof(uintptr_t), old, &old);

		if (addr==0)
			return 0;
		addr += offsets[i];
	}
	return addr;
}

unsigned long readDWORD(unsigned long ptrBase, int offset) {
	__try { return (*(unsigned long*)ptrBase + offset); }
	__except (EXCEPTION_EXECUTE_HANDLER) { return 0; }
}
float readFloat(unsigned long ptrBase, int offset) {
	__try { return (*(float*)ptrBase + offset); }
	__except (EXCEPTION_EXECUTE_HANDLER) { return 0; }
}

PVOID DetourVTable(void** vTable, int index, void* detour)
{
	PVOID ret = vTable[index];

	DWORD old;
	VirtualProtect(&(vTable[index]), sizeof(PVOID), PAGE_EXECUTE_READWRITE, &old);
	vTable[index] = detour;
	VirtualProtect(&(vTable[index]), sizeof(PVOID), old, &old);

	return ret;
}