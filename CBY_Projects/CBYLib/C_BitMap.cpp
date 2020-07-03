#include "C_BitMap.h"

bool C_BitMap::Load(const TCHAR* filename)
{
	HANDLE Bit_File = ::CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD ReadData;
	BITMAPFILEHEADER bf;
	ReadFile(Bit_File, &bf, sizeof(BITMAPFILEHEADER), &ReadData, NULL);
	int InfoData = bf.bfOffBits - sizeof(BITMAPFILEHEADER);
	BITMAPINFO* bif=(BITMAPINFO*)malloc(InfoData);
	ReadFile(Bit_File, bif, InfoData, &ReadData, NULL);
	PVOID BitVoid=nullptr;
	H_BitMap = CreateDIBSection(g_ScreenDC, bif, DIB_RGB_COLORS, &BitVoid, NULL, 0);

	ReadFile(Bit_File, BitVoid, bf.bfSize - bf.bfOffBits, &ReadData, NULL);
	free(bif);
	CloseHandle(Bit_File);
	//H_BitMap = (HBITMAP)LoadImage(g_hInstance, filename, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	MemDC = CreateCompatibleDC(g_ScreenDC);
	SelectObject(MemDC, H_BitMap);
	return true;
}

bool C_BitMap::Init()
{
	return true;
}
bool C_BitMap::Frame()
{
	return true;
}
bool C_BitMap::Render()									//기본 이미지 출력
{
	return true;
}
bool C_BitMap::Release()
{
	DeleteDC(MemDC);
	return true;
}

C_BitMap::C_BitMap()
{
}


C_BitMap::~C_BitMap()
{
}
