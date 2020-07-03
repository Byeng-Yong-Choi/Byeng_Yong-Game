// dllmain.cpp: DLL 응용 프로그램의 진입점을 정의합니다.
#include "header.h"
/////////////////    컴파일 주요 옵션 //////////////
// 1)C++ --> 언어 -->  준수모드 : 아니오 
// 2)C++ --> 일반 -->  경고를 오류로 처리  : 아니오
// 3)C:\Program Files\Autodesk\3ds Max 2016 SDK\maxsdk\help --> gencid.exe 실행하여 발급
//         #define kgcaExport_CLASS_ID	Class_ID(0x76be0a8b, 0x2e016579)
// 4)구성속성 -> 디버깅 -> 명령 -> C:\Program Files\Autodesk\3ds Max 2016\3dsmax.exe
////////////////////////////////////////////////////
extern ClassDesc2* GetExportDesc();

HINSTANCE hInstance;
int controlsInit = FALSE;


BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	/*switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		OutputDebugString(L"DLL_PROCESS_ATTACH");
	}break;
	case DLL_THREAD_ATTACH:
	{
		OutputDebugString(L"DLL_PROCESS_ATTACH");
	}break;
	case DLL_THREAD_DETACH:
	{
		OutputDebugString(L"DLL_PROCESS_ATTACH");
	}break;
	case DLL_PROCESS_DETACH:
	{
		OutputDebugString(L"DLL_PROCESS_ATTACH");
	}break;
	break;
	}
	return TRUE;*/
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		hInstance = hinstDLL;
		//DisableThreadLibraryCalls(hInstance);
	}
	return TRUE;
}

__declspec(dllexport) const TCHAR* LibDescription()
{
	return _T("CBYSkinExporter 100");
}

__declspec(dllexport) int LibNumberClasses()
{
	return 1;
}

__declspec(dllexport) ClassDesc* LibClassDesc(int i)
{
	switch (i) {
	case 0: return GetExportDesc();
	default: return 0;
	}
}

__declspec(dllexport) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

__declspec(dllexport) int LibInitialize(void)
{
//#pragma message(TODO("Perform initialization here."))
	return TRUE;
}

__declspec(dllexport) int LibShutdown(void)
{
//#pragma message(TODO("Perform un-initialization here."))
	return TRUE;
}

//TCHAR *GetString(int id)
//{
//	static TCHAR buf[256];
//
//	if (hInstance)
//		return LoadString(hInstance, id, buf, _countof(buf)) ? buf : NULL;
//	return NULL;
//}