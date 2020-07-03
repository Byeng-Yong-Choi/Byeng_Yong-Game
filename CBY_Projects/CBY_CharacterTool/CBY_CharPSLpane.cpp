// CBY_CharPSLpane.cpp: 구현 파일
//

#include "stdafx.h"
#include "CBY_CharacterTool.h"
#include "CBY_CharPSLpane.h"


// CBY_CharPSLpane

IMPLEMENT_DYNAMIC(CBY_CharPSLpane, CDockablePane)

CBY_CharPSLpane::CBY_CharPSLpane()
{

}

CBY_CharPSLpane::~CBY_CharPSLpane()
{
}


BEGIN_MESSAGE_MAP(CBY_CharPSLpane, CDockablePane)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// CBY_CharPSLpane 메시지 처리기
void CBY_CharPSLpane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_WndSetForm)
	{
		m_WndSetForm->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}


int CBY_CharPSLpane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_WndSetForm = CBY_CharPSLform::CreateOne(this);
	return 0;
}


int CBY_CharPSLpane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
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


