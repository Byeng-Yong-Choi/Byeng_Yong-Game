#pragma once
#include <string>


class GameRank
{
public:
	/*std::vector<float> m_Rankdata;
	float writeRank;
	int floatDataSize;
	HANDLE File;
	TCHAR ReadData[1000];
	TCHAR WriteData[1000];
	int data;
	DWORD HightDatasize;
	DWORD LowDatasize;
	DWORD datasize;*/
	bool Click;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	GameRank();
	~GameRank();
};

