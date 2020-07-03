// CBY_CreateMap.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_Tool.h"
#include "CBY_CreateMap.h"
#include "afxdialogex.h"


// CBY_CreateMap 대화 상자

IMPLEMENT_DYNAMIC(CBY_CreateMap, CDialogEx)

CBY_CreateMap::CBY_CreateMap(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CBY_CreateMap, pParent)
{

}

CBY_CreateMap::~CBY_CreateMap()
{
}

void CBY_CreateMap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBY_CreateMap, CDialogEx)
END_MESSAGE_MAP()


// CBY_CreateMap 메시지 처리기
