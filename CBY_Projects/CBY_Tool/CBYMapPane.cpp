// CBYMapPane.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_Tool.h"
#include "CBYMapPane.h"


// CBYMapPane

IMPLEMENT_DYNAMIC(CBYMapPane, CDockablePane)

CBYMapPane::CBYMapPane()
{

}

CBYMapPane::~CBYMapPane()
{
}


BEGIN_MESSAGE_MAP(CBYMapPane, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// CBYMapPane 메시지 처리기

void CBYMapPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_WndForm)
	{
		m_WndForm->SetWindowPos(NULL, 0, 0, cx, cy,SWP_NOZORDER);
	}
}


int CBYMapPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_WndForm = CBYMapForm::CreateOne(this);
	return 0;
}


int CBYMapPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(
			pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}
