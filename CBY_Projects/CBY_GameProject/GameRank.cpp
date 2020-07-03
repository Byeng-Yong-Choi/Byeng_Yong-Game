#include "GameRank.h"


bool GameRank::Init()
{
	
	return true;
}

bool GameRank::Frame()
{
	
	File = CreateFile(L"../../data/rank/ranking.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	//memset(ReadData, 0, sizeof(TCHAR) * 255);
	HightDatasize=GetFileSize(File, NULL);
	floatDataSize = HightDatasize / 4;
	m_Rankdata.clear();
	m_Rankdata.resize(floatDataSize);
	if (Click)
	{
		//ReadFile(File, ReadData, sizeof(TCHAR) * HightDatasize, &datasize, NULL);
		for (int i = 0; i < floatDataSize; i++)
		{
			ReadFile(File, &writeRank, sizeof(float), &datasize, NULL);
			m_Rankdata[i] = writeRank;
		}
		CloseHandle(File);
	}
	else
	{	
		/*memset(WriteData, 0, sizeof(TCHAR) * 255);
		ReadFile(File, ReadData, sizeof(TCHAR) * HightDatasize, &datasize, NULL);
		wcscat_s(WriteData, ReadData);
		swprintf_s(WriteData, L"플레이어 점수:%0.0f점\n", g_gametime);
		WriteFile(File, WriteData, lstrlen(WriteData)* sizeof(TCHAR), &datasize, NULL);*/
		
		for (int i = 0; i < floatDataSize; i++)
		{
			ReadFile(File, &writeRank, sizeof(float), &datasize, NULL);
			m_Rankdata[i] = writeRank;
		}
		writeRank=g_gametime;
		m_Rankdata.push_back(writeRank);
		/*for (int i = 0; i < m_Rankdata.size(); i++)
		{
			WriteFile(File, &m_Rankdata[i], sizeof(float), &datasize, NULL);
		}*/
		
		WriteFile(File, &m_Rankdata.back(), sizeof(float), &datasize, NULL);
		
		CloseHandle(File);
	}

	std::sort(m_Rankdata.begin(), m_Rankdata.end(), std::greater<float>());
	return true;
}

bool GameRank::Render()
{
	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCAND);
	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);
	BitBlt(g_offScreenDC, IX, IY, rtsrc.right, rtsrc.bottom, m_pMaskBitMap->MemDC, rtsrc.left, rtsrc.top, SRCINVERT);

	memset(ReadData, 0, sizeof(TCHAR) * 255);
	memset(WriteData, 0, sizeof(TCHAR) * 255);
	for (int i = 0; i < m_Rankdata.size(); i++)
	{
		swprintf_s(WriteData, L"%d등 플레이어 점수: %0.0f 점\n", i+1, m_Rankdata[i]);
		wcscat_s(ReadData, WriteData);
	}

	RECT rt={409, 140, Winrt.right, Winrt .bottom};
	SetBkMode(g_offScreenDC, TRANSPARENT);
	SetTextColor(g_offScreenDC, RGB(0, 0, 0));
	//DrawText(g_offScreenDC, ReadData, -1, &rt, DT_LEFT | DT_VCENTER);
	DrawText(g_offScreenDC, ReadData, -1, &rt, DT_LEFT | DT_VCENTER);
	

	return true;
}

bool GameRank::Release()
{
	return true;
}


GameRank::GameRank()
{
	data = 0;
	Click = false;
}


GameRank::~GameRank()
{
}
