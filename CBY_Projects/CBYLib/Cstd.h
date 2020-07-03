#pragma once
#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"fmod_vc.lib")
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include "CBYLinkList.h"
#include "CTime.h"
#include "CBY_Input.h"
#include "C_BitMap.h"
#include <time.h>
#include <math.h>
#include <list>
#include <map>
#include <vector>
#include <algorithm>


#define CBY_WINDOWHEAD int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#define CBY_WINDOWRUN(Class,b,c) {Class game;  srand((unsigned)time(NULL)); if(game.InitWindow(L#b,L#c)){game.GameRun();} return 1;
#define CBY_WINDOWTAIL }
#define CBY_RUN(Class,b,c) CBY_WINDOWHEAD CBY_WINDOWRUN(Class,b,c) CBY_WINDOWTAIL

#define Data_CalssName(Class) Class


extern HWND g_hwnd;
extern float g_SecondTime;
extern HINSTANCE g_hInstance;
extern HDC g_offScreenDC;
extern RECT Winrt;
extern HDC g_ScreenDC;
extern float g_gametime;

struct fPOINT
{
	float x;
	float y;
};

struct fRECT
{
	float left;
	float top;
	float right;
	float bottom;
};

struct fSphere
{
	fPOINT fpt;
	float radius;
};

static std::string wtm(std::wstring data)
{
	return std::string(data.begin(), data.end());
}