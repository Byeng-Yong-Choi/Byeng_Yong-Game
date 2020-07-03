#pragma once
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <time.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "C_Protocol.h"
#include <tchar.h>
#include "CDxHelperEx.h"
#include "CTime.h"

#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"fmod_vc.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable : 4005)

#define CBY_WINDOWHEAD int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#define CBY_WINDOWRUN(Class,b,c) {Class game;  srand((unsigned)time(NULL)); if(game.InitWindow(L#b,L#c)){game.GameRun();} return 1;
#define CBY_WINDOWTAIL }
#define CBY_RUN(Class,b,c) CBY_WINDOWHEAD CBY_WINDOWRUN(Class,b,c) CBY_WINDOWTAIL

#define Data_CalssName(Class) Class


extern HWND g_hwnd;
extern float g_SecondTime;
extern float g_gametime;
extern HINSTANCE g_hInstance;
extern RECT Winrt;

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

typedef std::basic_string<TCHAR> T_STR;
typedef std::basic_string<wchar_t> W_STG;
typedef std::basic_string<char>  C_STR;
typedef std::vector<T_STR>		T_STR_VECTOR;
typedef std::basic_string<TCHAR>::iterator	T_ITOR;
typedef std::basic_string<wchar_t>::iterator	W_ITOR;
typedef std::basic_string<char>::iterator	C_ITOR;




