// CBY_ObjectLoad.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_Tool.h"
#include "CBY_ObjectLoad.h"
#include "afxdialogex.h"


// CBY_ObjectLoad 대화 상자

IMPLEMENT_DYNAMIC(CBY_ObjectLoad, CDialogEx)

CBY_ObjectLoad::CBY_ObjectLoad(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CBY_ObjectLoad, pParent)
{

}

CBY_ObjectLoad::~CBY_ObjectLoad()
{
}

void CBY_ObjectLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBY_ObjectLoad, CDialogEx)
END_MESSAGE_MAP()


// CBY_ObjectLoad 메시지 처리기
