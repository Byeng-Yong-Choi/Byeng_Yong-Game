// CBY_ObjPane.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_ObjTool.h"
#include "CBY_ObjPane.h"


// CBY_ObjPane

IMPLEMENT_DYNAMIC(CBY_ObjPane, CDockablePane)

CBY_ObjPane::CBY_ObjPane()
{

}

CBY_ObjPane::~CBY_ObjPane()
{
}


BEGIN_MESSAGE_MAP(CBY_ObjPane, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// CBY_ObjPane 메시지 처리기
void CBY_ObjPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_WndForm)
	{
		m_WndForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}


int CBY_ObjPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_WndForm = CBY_ObjView::CreateOne(this);
	return 0;
}


int CBY_ObjPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
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

