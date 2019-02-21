class CInPacket {
	unsigned char pad1[10];
	unsigned char* pBuffer; //0x10
//	padding(0x8); //0x14
	unsigned long nSize; //0x20
//	padding(0x8); //0x24
	unsigned long nPad; //0x30
};