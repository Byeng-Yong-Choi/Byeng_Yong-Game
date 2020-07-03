// CBY_ObjFrameSet.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_CharacterTool.h"
#include "CBY_ObjFrameSet.h"
#include "afxdialogex.h"


// CBY_ObjFrameSet 대화 상자

IMPLEMENT_DYNAMIC(CBY_ObjFrameSet, CDialogEx)

CBY_ObjFrameSet::CBY_ObjFrameSet(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CBY_CharSetFrom, pParent)
{

}

CBY_ObjFrameSet::~CBY_ObjFrameSet()
{
}

void CBY_ObjFrameSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBY_ObjFrameSet, CDialogEx)
END_MESSAGE_MAP()


// CBY_ObjFrameSet 메시지 처리기
