#pragma once
uintptr_t readPointerOffset(uintptr_t ptr, std::vector<unsigned int> offsets);

unsigned long readDWORD(unsigned long ptrBase, int offset=0);
float readFloat(unsigned long ptrBase, int offset=0);